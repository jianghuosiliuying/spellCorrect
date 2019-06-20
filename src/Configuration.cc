#include "../include/Configuration.h"
#include <iostream>
#include <fstream>

using namespace std;

namespace mm
{
Configuration::Configuration(const string& filepath)
:filepath_(filepath)
{
    ifstream ifs(filepath_);
    string s1,s2;
    while(ifs>>s1>>s2)
    {
        //cout<<s1<<" "<<s2<<endl;
        configMap_.insert(make_pair(s1,s2));
    }
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

}//end of namespace mm
