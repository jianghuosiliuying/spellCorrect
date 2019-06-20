#include "../include/DictProducer.h"
#include "../include/SplitTool.h"
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

namespace mm
{
DictProducer::DictProducer(const string & dir)
:dir_(dir)
{
    DIR * d=opendir(dir_.c_str());
    if(d==nullptr){
        perror("opendir");
    }
    struct dirent *p;
    while((p=readdir(d)))
    {
        char path[512]={0};
        if(strstr(p->d_name,".txt")){
            sprintf(path,"%s/%s",dir_.c_str(),p->d_name);
            file_.push_back(path);
        }
    }
    closedir(d);
}
void DictProducer::build_dict()
{
    for(auto & file:file_)
    {
        string word;
        ifstream ifs(file);
        while(ifs>>word)
        {
            while(!word.empty()&&(ispunct(word[0])||isdigit(word[0])))
            {   word.erase(word.begin());  }
            if(word.empty())  continue;
            while(ispunct(word[word.size()-1]))
                word.erase(word.end()-1);
            if(!isalpha(word[0]))
                continue;
            else{
                for(size_t i=0;i!=word.size();++i)
                {
                    word[i]=tolower(word[i]);
                }
                dict_[word]++;
            }
        }
    }
}
void DictProducer::store_dict(const string filepath)
{
    //string file=filepath+"/mydict.dat";
    ofstream ofs(filepath);
    for(auto it=dict_.begin();it!=dict_.end();++it)
    {
        ofs<<it->first<<" "<<it->second<<endl;
    }
    ofs.close();
}
void DictProducer::show_dict()const
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

}//end of namespace mm
