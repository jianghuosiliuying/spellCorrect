#include "../include/Configuration.h"
#include "../include/DictProducer.h"
#include "../include/IndexProducer.h"
#include "../include/Mydict.h"
#include <iostream>
//用来生成词典文件和索引文件的main方法
using namespace std;
using namespace mm;

int main(int argc,char** argv)
{
    if(argc!=2)
    {
        cout<<"error args!"<<endl;
        return -1;
    }
    string confpath=argv[1];
    Configuration conf(confpath);
    conf.show_conf();//输出配置文件
    map<string,string> con=conf.getConfigMap();
    string dir=con.find("dir")->second;//语料库,以.txt结尾
    DictProducer dict(dir);
    dict.build_dict();
    string filepath=con.find("dict")->second;//英文词频库地址+文件名
    dict.store_dict(filepath);
    //dict.show_dict();//输出前10个单词
    string indexpath=con.find("index")->second;//英文索引库地址+文件名
    IndexProducer index(filepath,indexpath);
    index.read_dic();
    index.build_index();
    index.store_index();
    //index.show_index();//输出10个字母和对应前10个index
    Mydict * mydict=Mydict::createMydict();
    mydict->initEn(filepath,indexpath);
    mydict->show_dict();
    mydict->show_index();
    return 0;
}

