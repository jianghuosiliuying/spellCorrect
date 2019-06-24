#include "../include/Cache.h"
#include <iostream>

using namespace std;

namespace mm
{
Cache::Cache(unordered_map<string,string> & mainMap)
{   table_=mainMap;  }//将主线程Map构建为Cache对象

void Cache::addElement(string word,string json)
{
    table_.insert(make_pair(word,json));
}
unordered_map<string,string> & Cache::getCacheMap()
{   return table_;  }


}//end of namespace mm

