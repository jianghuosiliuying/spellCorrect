#include "../include/Cache.h"
#include <iostream>

using namespace std;

namespace mm
{
void Cache::addElement(string word,string json)
{
    if(table_.find(word)==table_.end())
        table_.insert(make_pair(word,json));
}


}//end of namespace mm

