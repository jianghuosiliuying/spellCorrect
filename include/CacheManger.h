#pragma once

#include <vector>
#include <string>

using namespace std;

namespace mm
{
class Cache;
class CacheManger
{
public:
    static CacheManger * createCacheManger();
    static void destory();
    void init(string);
    Cache & getCache(size_t);
    void periodicUpdate();
private:
    CacheManger();
    ~CacheManger() {}
private:
    static CacheManger * pCacheManger_;
    vector<Cache> cacheList_;
};

}//end of namespace mm
