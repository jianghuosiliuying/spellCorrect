#pragma once

#include <map>
#include <set>
#include <vector>

using namespace std;

namespace mm
{
class IndexProducer
{
public:
    IndexProducer(const string & dicpath,const string & indexpath);
    void read_dic();//读取词典
    void build_index();
    int nBytesCode(const char ch);//获取每个字所占字节数
    void store_index();
    void show_index()const;
private:
    string dicpath_;//词典路径
    string indexpath_;//索引路径
    vector<pair<string,int> > vecdic_;//存储词典文件内容
    map<string,set<int> > mapIndex_;//存储字母和相关vector下标
};

}//end of namespace mm
