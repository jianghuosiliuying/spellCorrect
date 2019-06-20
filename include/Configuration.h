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
    Configuration(const string&);
    map<string,string> & getConfigMap();
    void show_conf()const;
private:
    string filepath_;//配置文件路径
    map<string,string> configMap_;//配置文件内容
};

}//end of namespace mm
