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
    CacheManger() {}
    ~CacheManger() {}
    void init(string);
    Cache & getCache(size_t);
    void periodicUpdate();
private:
    vector<Cache> cacheList_;
};

}//end of namespace mm
