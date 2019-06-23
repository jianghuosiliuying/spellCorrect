#pragma once

#include <vector>
#include <map>
#include <set>
#include <string>
#include <iostream>

using namespace std;

namespace mm
{
class Mydict
{
public:
    static Mydict * createMydict();
    static void destory();
    void initEn(const string dictEnPath,const string indexEnPath);//初始化英文词典和索引表
    void initCn(const string dictCnPath,const string indexCnPath);//初始化中文词典和索引表
    vector<pair<string,int> > & getDict();//获取词典
    map<string,set<int> > & getIndexTable();//获取索引表
    void show_dict()const;//查看词典
    void show_index()const;
private:
    Mydict(){   cout<<"Mydict()"<<endl;  }
    ~Mydict(){}

private:
    static Mydict * pMydict_;
    vector<pair<string,int> > dict_;//词典
    map<string,set<int> > indextable_;//索引表
};

}//end of namespace mm
