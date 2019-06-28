#include "./cppjieba/Jieba.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;
using namespace cppjieba;

const char * const DICT_PATH="./cppjieba/dict/jieba.dict.utf8";//最大概率法分词所使用的词典路径
const char * const HMM_PATH="./cppjieba/dict/hmm_model.utf8";//隐式马尔科夫模型分词所使用的词典路径
const char * const USER_DICT_PATH="./cppjieba/dict/user.dict.utf8";//用户自定义词典路径
const char * const IDF_PATH="./cppjieba/dict/idf.utf8";//IDF路径
const char * const STOP_WORD_PATH="./cppjieba/dict/stop_words.utf8";//停用词路径

int main()
{
    //string str="上善若水，厚德载物。";
    string str;
    ifstream ifs("test.txt");
    Jieba jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH);
    while(ifs>>str)
    {//默认就是每次读一行
        cout<<str<<endl;
        //Jieba jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH);
        vector<string> result;
        jieba.Cut(str,result,true);
        cout<<"分词结果："<<endl;
        for(auto & re:result)
        {
            //cout<<re.size()<<" "<<re<<" ";
            cout<<re<<" ";
        }
        cout<<endl;
    }
    return 0;
}

