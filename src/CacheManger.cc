#pragma once

#include "../include/CacheManger.h"
#include "../include/Configuration.h"
//#include "../include/Cache.h"
#include "../include/MutexLock.h"
#include <fstream>

using namespace std;

namespace mm
{
CacheManger * CacheManger::init() 
{
    pconf_=Configuration::createConfiguration();
    cacheNum_=stoi(pconf_->getConfigMap()["threadNum"])+1;//cache数量为子线程数目+1
    for(int i=0;i!=cacheNum_;++i)
    {
        cacheList_.push_back(Cache());//临时对象有移动赋值控制函数吗
    }//cache数量为子线程和主线程总和
    readFromFile();//初始化cache
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
        cacheList_[0].addElement(word,json);//0号cache为主线程cache
    }
    ifs.close();
    for(int i=1;i<cacheNum_;++i)
    {//初始化所有线程
        cacheList_[i]=cacheList_[0];
    }
    //Cache tmp=getCache(4);
    //for(auto & i:tmp.getCacheMap())
    //{
    //    cout<<i.first<<" "<<i.second<<endl;
    //}
}
void CacheManger::writeToFile()
{
    cout<<"i will write cache to file."<<endl;
    ofstream ofs(cacheFile_);
    Cache tmp=getCache(0);//获取0号cache
    for(auto & k:tmp.getCacheMap())
    {
        ofs<<k.first<<" "<<k.second<<endl;
    }
    ofs.close();
}
Cache & CacheManger::getCache(size_t idx)
{
    return cacheList_[idx];
}
void CacheManger::periodicUpdate()
{
    cout<<"i will update cache to file."<<endl;
    Cache mainCache=getCache(0);
    unordered_map<string,string> mainMap=mainCache.getCacheMap();
    {
        MutexLockGuard autolock(mutex_);
        for(int i=1;i<cacheNum_;++i)
        {
            unordered_map<string,string> tmp=getCache(i).getCacheMap();
            for(auto & j:tmp)
            {//遍历子线程map，插入主线程map
                mainMap.insert(j);
            }
        }
    }//缩小锁的范围
    cacheList_[0]=Cache(mainMap);//将主map赋给0号位置
    {
        MutexLockGuard autolock(mutex_);
        for(int i=1;i<cacheNum_;++i)
        {//更新所有子线程的cache
            cacheList_[i]=cacheList_[0];
        }
    }//减小锁的范围
    writeToFile();//将更新后的cache写回文件
}


CacheManger * CacheManger::pCacheManger_=createCacheManger();//饱汉模式

}//end of namespace mm
