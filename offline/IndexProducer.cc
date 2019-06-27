#include "../include/IndexProducer.h"
#include <iostream>
#include <fstream>

using namespace std;

namespace mm
{
IndexProducer::IndexProducer(const string& dicpath,const string& indexpath)
:dicpath_(dicpath),indexpath_(indexpath){}
void IndexProducer::read_dic()
{
    string word;
    int wordnum=0;
    ifstream ifs(dicpath_);
    cout<<"will read_dic. "<<dicpath_<<endl;
    while(ifs>>word>>wordnum)
    {
        vecdic_.push_back(make_pair(word,wordnum));
    }
    cout<<"read_dic finish."<<endl;
    ifs.close();
}
int IndexProducer::nBytesCode(const char ch)
{//计算每个字的长度
    if(ch & (1<<7))
    {
        int nBytes=1;
        for(int i=0;i!=6;++i)
        {//utf-8最大占6个字节
            if(ch & (1<<(6-i))){
                ++nBytes;
            }else{
                break;
            }
        }
        return nBytes;
    }
    return 1;
}
void IndexProducer::build_index()
{
    cout<<"will build_index."<<endl;
    string word;
    int location=0;
    for(auto & vec:vecdic_)
    {
        word=vec.first;
        int nBytes;
        string ch;//单个字
        size_t cur=0;
        while(cur!=word.size())
        {
            nBytes=nBytesCode(word[cur]);//获取当前字所占字符数
            ch=word.substr(cur,nBytes);//将word前nBytes个字符解析成单个字
            //cout<<ch<<endl;
            cur+=nBytes;
            mapIndex_[ch].insert(location);//给对应字的set中插入vector的下标
        }
        location++;
    }
    cout<<"build_index finish."<<endl;
}
void IndexProducer::store_index()
{
    ofstream ofs(indexpath_);
    cout<<"will store_index "<<indexpath_<<endl;
    for(auto & index:mapIndex_)
    {
        ofs<<index.first<<endl;
        for(auto & local:index.second)
        {
            ofs<<local<<" ";
        }
        ofs<<endl;
    }
    ofs.close();
    cout<<"store_index finish."<<endl;
}
void IndexProducer::show_index()const
{
    int wordcnt=10;
    for(auto it=mapIndex_.begin();it!=mapIndex_.end();++it)
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

}//end of namespace mm

