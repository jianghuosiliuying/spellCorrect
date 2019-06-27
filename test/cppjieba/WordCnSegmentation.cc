#include "WordCnSegmentation.h"
#include <iostream>

using namespace std;

namespace mm
{
WordCnSegmentation::WordCnSegmentation()
:_jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH)//初始化jieba类对象
{   
    cout<<"cppjieba init."<<endl;  
}
vector<string> WordCnSegmentation::operator()(const string str)
{
    vector<string> words;
    _jieba.CutAll(str, words);//FullSegment
    //_jieba.Cut(str, words);//Segment
    return words;
}

}//end of namespace mm
