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
//    Cache(const Cache & cache);
    ~Cache() {}
    //Cache & operator=(const Cache & cache);
    void addElement(string,string);
    unordered_map<string,string> & getCacheMap();
    //void readFromFile(string);
    //void writeToFile(string);
    void update(const Cache &);
private:
    unordered_map<string,string> table_;
};

}//end of namespace mm
