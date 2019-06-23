#pragma once

#include "../include/CacheManger.h"
#include "../include/Configuration.h"
#include "../include/Cache.h"

namespace mm
{
CacheManger * CacheManger::init() 
{
    pconf_=Configuration::createConfiguration();
    int cacheNum=stoi(pconf_->getConfigMap()["cacheNum"]);
    cacheList_.reserve(cacheNum);//cache数量为子线程和主线程总和
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


CacheManger * CacheManger::pCacheManger_=createCacheManger();//饱汉模式

}//end of namespace mm
