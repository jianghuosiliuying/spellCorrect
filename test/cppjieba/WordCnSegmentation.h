#pragma once

#include "./cppjieba/Jieba.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;

const char * const DICT_PATH="./cppjieba/dict/jieba.dict.utf8";//最大概率法分词所使用的词典路径
const char * const HMM_PATH="./cppjieba/dict/hmm_model.utf8";//隐式马尔科夫模型分词所使用的词典路径
const char * const USER_DICT_PATH="./cppjieba/dict/user.dict.utf8";//用户自定义词典路径
const char * const IDF_PATH="./cppjieba/dict/idf.utf8";//IDF路径
const char * const STOP_WORD_PATH="./cppjieba/dict/stop_words.utf8";//停用词路径

namespace mm
{
class WordCnSegmentation//使用jieba词库进行分词
{
public:
    WordCnSegmentation();
    vector<string> operator()(const string str);//返回str的分词结果
private:
    cppjieba::Jieba _jieba;
};

}//end of namespace mm
