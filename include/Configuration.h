#pragma once

#include <string>
#include <map>

using std::string;
using std::map;

namespace mm
{
class Configuration
{
public:
    static Configuration * createConfiguration();
    static void destroy();
    Configuration * initConf(const string&);
    map<string,string> & getConfigMap();
    void show_conf()const;
private:
    Configuration();
    ~Configuration(){}
private:
    static Configuration * pConf_;
    //string filepath_;//配置文件路径
    map<string,string> configMap_;//配置文件内容
};

}//end of namespace mm
