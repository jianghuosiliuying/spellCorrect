#pragma once

#include <unordered_map>
#include <string>

using namespace std;

namespace mm
{
class Cache
{
public:
    Cache() {}
    Cache(unordered_map<string,string> & mainMap);
//    Cache(const Cache & cache);
    void addElement(string,string);
    unordered_map<string,string> & getCacheMap();
private:
    unordered_map<string,string> table_;
};

}//end of namespace mm
