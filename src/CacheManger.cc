#pragma once

#include "../include/CacheManger.h"
#include "../include/Configuration.h"
#include "../include/MutexLock.h"
#include <fstream>

using namespace std;

namespace mm
{
CacheManger::CacheManger()
:pconf_(Configuration::createConfiguration())
,cacheCap_(stoi(pconf_->getConfigMap().find("cacheCap")->second))
,tmplist_(cacheCap_)
{   cout<<"CacheManger()"<<endl;  }

CacheManger * CacheManger::init() 
{
    cout<<"init all cache."<<endl;
    cacheNum_=stoi(pconf_->getConfigMap()["threadNum"]);//cache数量为子线程数目
    for(int i=0;i!=cacheNum_;++i)
    {
        cacheList_.push_back(Cache(cacheCap_));//临时对象有移动赋值控制函数吗
    }//cache数量为子线程总和
    readFromFile();//初始化cache
    cout<<"init main cache finish."<<endl;
    return pCacheManger_;
}
CacheManger * CacheManger::createCacheManger()
{
    if(pCacheManger_==nullptr){
        pCacheManger_=new CacheManger();
        atexit(destory);
    }
    return pCacheManger_;
}
void CacheManger::destory()
{
    if(pCacheManger_)
        delete pCacheManger_;
}
void CacheManger::readFromFile()
{
    string word,json;
    cacheFile_=pconf_->getConfigMap().find("cache")->second;
    ifstream ifs(cacheFile_);
    while(ifs>>word>>json)
    {
        tmplist_.addElement(word,json);//主cache
    }
    ifs.close();
}
#if 0
void CacheManger::addHotDate(string word,string json)
{//添加热数据
    tmplist_.addElement(word,json);
}
string CacheManger::getHotData(string word)
{
    return tmplist_.searchElement(word);
}
#endif
void CacheManger::writeToFile()
{
    cout<<"i will write cache to file."<<endl;
    ofstream ofs(cacheFile_);
    list<pair<string,string>> & tmplist=tmplist_.getCacheList();
    for(auto & k:tmplist)
    {
        ofs<<k.first<<" "<<k.second;//每个json语句自带换行
        //cout<<k.first<<" "<<k.second<<endl;
    }
    ofs.close();
    cout<<"write cache to file finish."<<endl;
}
Cache & CacheManger::getCache(size_t idx)
{
    if(idx==0)
        return tmplist_;
    else
        return cacheList_[idx-1];
}
void CacheManger::periodicUpdate()
{
    cout<<"i will read subthread cache."<<endl;
    {
        MutexLockGuard autolock(mutex_);
        for(int i=1;i<=cacheNum_;++i)
        {
            Cache tmp(cacheCap_);
            tmp=cacheList_[i-1];//读取子线程的热数据
            cacheList_[i-1]=Cache(cacheCap_);//给子线程一个新cache，清空原有热数据
            list<pair<string,string>> & tmplist=tmp.getCacheList();
            for(auto it=tmplist.rbegin();it!=tmplist.rend();++it)
            {//从尾部向头部遍历链表
                tmplist_.addElement(it->first,it->second);//直接插入到主cache
            }
        }
    }//缩小锁的范围
    cout<<"i read subthread cache finish."<<endl;
    writeToFile();//将更新后的cache写回文件
}

//CacheManger * CacheManger::pCacheManger_=createCacheManger();//饱汉模式
CacheManger * CacheManger::pCacheManger_=nullptr;//饿汉模式

}//end of namespace mm
