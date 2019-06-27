#include "../include/Mydict.h"
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

namespace mm
{

Mydict * Mydict::createMydict()
{
    if(pMydict_==nullptr){
        pMydict_=new Mydict();
        atexit(destory);
    }
    return pMydict_;
}
void Mydict::destory()
{
    if(pMydict_)
        delete pMydict_;
}
void Mydict::init(const string dictPath,const string indexPath)
{
    string word;
    int wordcnt;
    cout<<"will build dict."<<dictPath<<endl;
    ifstream ifsdict(dictPath);//构建词典
    while(ifsdict>>word>>wordcnt)
    {
        dict_.push_back(make_pair(word,wordcnt));
    }
    ifsdict.close();
    string line;
    int location,k=1;
    cout<<"will build index."<<indexPath<<endl;
    ifstream ifsindex(indexPath);//构建索引表
    while(getline(ifsindex,line))
    {
        stringstream ss(line);
        if(k%2!=0)//奇数行
        {
            while(ss>>word);//奇数行只可能有一个词
        }else{
            while(ss>>location)
            {
                indextable_[word].insert(location);
            }
        }
        ++k;
    }
    ifsindex.close();
}
vector<pair<string,int> > & Mydict::getDict()
{
    return dict_;
}
map<string,set<int> > & Mydict::getIndexTable()
{
    return indextable_;
}
void Mydict::show_dict()const
{
    int cnt=10;
    for(auto it=dict_.begin();it!=dict_.end();++it)
    {
        if(cnt>0)
        {
            cout<<it->first<<" "<<it->second<<endl;
            --cnt;
        }else
            break;
    }
}
void Mydict::show_index()const
{
    int wordcnt=10;
    for(auto it=indextable_.begin();it!=indextable_.end();++it)
    {
        if(wordcnt>0)
        {
            int indexcnt=10;
            cout<<it->first<<endl;
            for(auto iter=it->second.begin();iter!=it->second.end();++iter)
            {
                if(indexcnt>0)
                {
                    cout<<*iter<<" ";
                    --indexcnt;
                }else{
                    cout<<endl;
                    break;
                }
            }
            --wordcnt;
        }else{
            break;
        }
    }
}

Mydict * Mydict::pMydict_=createMydict();//饱汉模式

}//end of namespace mm
