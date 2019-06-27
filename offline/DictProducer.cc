#include "../include/DictProducer.h"
#include "../include/cppjieba/Jieba.hpp"
#include "../include/Configuration.h"
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

namespace mm
{

const char * const DICT_PATH="../include/cppjieba/dict/jieba.dict.utf8";//最大概率法分词所使用的词典路径
const char * const HMM_PATH="../include/cppjieba/dict/hmm_model.utf8";//隐式马尔科夫模型分词所的词典路径
const char * const USER_DICT_PATH="../include/cppjieba/dict/user.dict.utf8";//用户自定义词典路径
const char * const IDF_PATH="../include/cppjieba/dict/idf.utf8";//IDF路径
const char * const STOP_WORD_PATH="../include/cppjieba/dict/stop_words.utf8";//停用词路径

DictProducer::DictProducer()
{//读取英文语料目录的文件名。保存到vector中
    pconf_=Configuration::createConfiguration();
    string dirEn=pconf_->getConfigMap().find("enLib")->second;//英文语料库,以.txt结尾
    DIR * d1=opendir(dirEn.c_str());
    if(d1==nullptr){
        perror("opendir");
    }
    struct dirent *p1;
    while((p1=readdir(d1)))
    {
        char path[512]={0};
        if(strstr(p1->d_name,".txt")){
            sprintf(path,"%s/%s",dirEn.c_str(),p1->d_name);
            fileEn_.push_back(path);
        }
    }
    closedir(d1);
    string dirCn=pconf_->getConfigMap().find("cnLib")->second;//中文语料库,以.txt结尾
    DIR * d2=opendir(dirCn.c_str());
    if(d2==nullptr){
        perror("opendir");
    }//读取中文语料目录的文件名。保存到vector中
    struct dirent *p2;
    while((p2=readdir(d2)))
    {
        char path[512]={0};
        if(strstr(p2->d_name,".txt")){
            sprintf(path,"%s/%s",dirCn.c_str(),p2->d_name);
            fileCn_.push_back(path);
        }
    }
    closedir(d2);
}
void DictProducer::build_Endict()
{//构建英文词典文件
    for(auto & file:fileEn_)
    {
        cout<<file<<endl;
        string word1;
        ifstream ifs(file);
        while(ifs>>word1)
        {
	    string word;
            for(auto & k:word1)
            {
                if(isalpha(k)){
                    word.push_back(k);
                }//只存单词中是字母的字符
            }
            if(word.empty())  {
		        continue;
	        }
            for(size_t i=0;i!=word.size();++i)
            {
                word[i]=tolower(word[i]);
            }
            ++dict_[word];
        }
        ifs.close();
    }
}
void DictProducer::build_Cndict()
{//构建中文词典文件
    using namespace cppjieba;
    for(auto & file:fileCn_)
    {//遍历每份文件
        cout<<file<<endl;
        ifstream ifs(file);
        string s;
        while(ifs>>s)
        {//读中文进行分词
            //cout<<"will jieba"<<endl;
            Jieba jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH);
            vector<string> result;
            jieba.Cut(s,result,true);
            for(auto & word:result)
            {
                ++dict_[word];
            }
        }
        ifs.close();
    }
}
void DictProducer::store_dict(const string filepath)
{
#if 0
    map<string,int> dict=dictEn_;
    for(auto & k:dictCn_)
    {//合并中英文词典
        ++dict[k.first];
    }
#endif
    cout<<"will save dict."<<endl;
    ofstream ofs(filepath);//一次性存储新内容
    for(auto it=dict_.begin();it!=dict_.end();++it)
    {//存词典
        ofs<<it->first<<" "<<it->second<<endl;
    }
    cout<<"dict save finish."<<endl;
#if 0
    for(auto it=dictEn_.begin();it!=dictEn_.end();++it)
    {//存英文词典
        ofs<<it->first<<" "<<it->second<<endl;
    }
    cout<<"dictEn save finish."<<endl;
    for(auto it=dictCn_.begin();it!=dictCn_.end();++it)
    {//存中文词典
        ofs<<it->first<<" "<<it->second<<endl;
    }
    cout<<"dictCn save finish."<<endl;
#endif
    ofs.close();
}

}//end of namespace mm
