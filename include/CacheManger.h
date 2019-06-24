#pragma once

#include "Cache.h"
#include "MutexLock.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

namespace mm
{
class MutexLock;
class Configuration;
class CacheManger
{
public:
    static CacheManger * createCacheManger();
    static void destory();
    CacheManger * init();//初始化cache数量，并首次同步cache
    void readFromFile();//从文件中读取cache数据
    void writeToFile();//将cache数据写到文件
    //void update();//更新cache
    Cache & getCache(size_t);//子线程通过线程编号获取cache
    void periodicUpdate();//周期更新cache
private:
    CacheManger(){  cout<<"CacheManger()"<<endl;  }
    ~CacheManger() {}
private:
    static CacheManger * pCacheManger_;
    Configuration * pconf_;
    string cacheFile_;//cache文件存放路径
    int cacheNum_;//cache数目
    MutexLock mutex_;
    vector<Cache> cacheList_;
};

}//end of namespace mm
