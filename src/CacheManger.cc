#pragma once

#include "../include/CacheManger.h"
#include "../include/Configuration.h"
#include "../include/Cache.h"

namespace mm
{
CacheManger::CacheManger() 
{
    Configuration * pconf=Configuration::createConfiguration();
    cacheList_.reserve();
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
