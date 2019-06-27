#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

namespace mm
{
class Configuration;
class DictProducer
{
public:
    DictProducer();

    void build_Endict();//创建英文词典
    void build_Cndict();//创建中文词典
    void store_dict(const string filepath);//将词典写入文件
private:
    vector<string> fileEn_;//英文语料库文件的绝对路径集合
    vector<string> fileCn_;//中文语料库文件的绝对路径集合
    //map<string,int> dictEn_;//英文单词和词频
    map<string,int> dict_;//中文单词和词频
    Configuration * pconf_;
};

}//end of namespace mm
