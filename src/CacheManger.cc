#pragma once

#include "../include/CacheManger.h"
#include "../include/Configuration.h"
#include "../include/Cache.h"
#include <iostream>
#include <fstream>

using namespace std;

namespace mm
{
CacheManger * CacheManger::init() 
{
    pconf_=Configuration::createConfiguration();
    int cacheNum=stoi(pconf_->getConfigMap()["cacheNum"]);
    for(int i=0;i!=cacheNum;++i)
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
    string cacheFile=pconf_->getConfigMap().find("cache")->second;
    ifstream ifs(cacheFile);
    while(ifs>>word>>json)
    {
        cacheList_[0].addElement(word,json);
    }
    int thNum=stoi(pconf_->getConfigMap()["cacheNum"]);
    for(int i=1;i<thNum;++i)
    {
        cacheList_[i]=cacheList_[0];
    }
    Cache tmp=getCache(4);
    for(auto & i:tmp.getCacheMap())
    {
        cout<<i.first<<" "<<i.second<<endl;
    }
}
Cache & CacheManger::getCache(size_t idx)
{
    return cacheList_[idx];
}


CacheManger * CacheManger::pCacheManger_=createCacheManger();//饱汉模式

}//end of namespace mm
