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
    while(ifs>>word>>wordnum)
    {
        vecdic_.push_back(make_pair(word,wordnum));
    }
    ifs.close();
}
void IndexProducer::build_index()
{
    string word;
    int location=0;
    for(auto & vec:vecdic_)
    {
        word=vec.first;
        int sz=vec.first.size();
        for(int i=0;i!=sz;++i)
        {
            if(isalpha(word[i]))
                mapIndex_[string(1,word[i])].insert(location);
        }
        location++;
    }
}
void IndexProducer::store_index()
{
    ofstream ofs(indexpath_);
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

