#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

namespace mm
{
class SplitTool;
class DictProducer
{
public:
    DictProducer(const string&);
    DictProducer(const string&,SplitTool*);//中文

    void build_dict();//创建英文词典
    void build_cn_dict();//创建中文词典
    void store_dict(const string filepath);//将词典写入文件
    void show_files()const;//查看文件路径
    void show_dict()const;//查看词典
private:
    void get_files();//获取文件的绝对路径
    void push_dict(const string & word);//存储某个单词
private:
    string dir_;//语料库文件存放路径
    vector<string> file_;//语料库文件的绝对路径集合
    map<string,int> dict_;//词和词频
    SplitTool * splitTool;//分词工具
};

}//end of namespace mm
