#include "../include/Configuration.h"
#include "../include/DictProducer.h"
#include "../include/IndexProducer.h"
#include "../include/Mydict.h"
//用来生成词典文件和索引文件的main方法
using namespace std;
using namespace mm;

int main()
{
    cout<<"1"<<endl;
    Configuration * pconf=Configuration::createConfiguration();
    pconf->initConf("../conf");
    cout<<"1"<<endl;
    map<string,string> con=pconf->getConfigMap();
    string filepath=con.find("dict")->second;//英文词频库地址+文件名
    cout<<"1"<<endl;
    //pconf->show_conf();//输出配置文件
#if 0
    DictProducer dict;//构建中英文词典目录
    dict.build_Endict();//构建英文词典
    dict.build_Cndict();//构建中文词典
    dict.store_dict(filepath);//存中文和英文词频
#endif
#if 1
    cout<<"1"<<endl;
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
#endif
    return 0;
}

