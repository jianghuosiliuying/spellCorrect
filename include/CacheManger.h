#pragma once

#include <vector>
#include <string>
#include <iostream>

using namespace std;

namespace mm
{
class Cache;
class Configuration;
class CacheManger
{
public:
    static CacheManger * createCacheManger();
    static void destory();
    CacheManger * init();//初始化cache数量，并首次同步cache
    Cache & getCache(size_t);
    void periodicUpdate();
private:
    CacheManger(){  cout<<"CacheManger()"<<endl;  }
    ~CacheManger() {}
private:
    static CacheManger * pCacheManger_;
    Configuration * pconf_;
    vector<Cache> cacheList_;
};

}//end of namespace mm
