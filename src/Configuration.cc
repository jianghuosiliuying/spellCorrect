#include "../include/Configuration.h"
#include <iostream>
#include <fstream>

using namespace std;

namespace mm
{
Configuration* Configuration::initConf(const string & filepath)
{
    ifstream ifs(filepath);
    string s1,s2;
    while(ifs>>s1>>s2)
    {
        configMap_.insert(make_pair(s1,s2));
    }
    return pConf_;
}
Configuration * Configuration::createConfiguration()
{
    if(pConf_==nullptr){
        pConf_=new Configuration();
        atexit(destroy);
    }
    return pConf_;
}
void Configuration::destroy()
{
    if(pConf_)
        delete pConf_;
}
map<string,string> & Configuration::getConfigMap()
{
    return configMap_;
}
void Configuration::show_conf()const
{
    for(auto & i:configMap_)
    {
        cout<<i.first<<" "<<i.second<<endl;
    }
}

static Configuration * pconf=nullptr;//饿汉模式,需先于线程前创建，否则可能存在线程安全

}//end of namespace mm
