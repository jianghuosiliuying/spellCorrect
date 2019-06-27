#if 0
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif
#endif
#include "../../include/json/json.h"
#include <iostream>
#include <string>
#include <memory>

using namespace std;

string createJson()
{
    Json::Value data;
    //ostringstream os;
    data["word"].append("wordld");
    //data[0]="world";
    //data[1]="world";
    data["word"].append("world");
    data["word"].append("你好");
    //data["word"]="fine";
    //data["word"]="你好";
    //data[4]="hello";//0.5版json只能从1开始
#if 0
    Json::StreamWriterBuilder writer;
    unique_ptr<Json::StreamWriter> jsonwriter(writer.newStreamWriter());
    jsonwriter->write(root,&os);
    string strData=os.str();
    cout<<strData<<endl;;
    //const char *str=strData.c_str();
    //string str1(str);
#endif
    //string strData=root.toStyledString();//与上边注释的功能相同，带输出格式的字符串
#if 1
    Json::FastWriter writer;
    string strData=writer.write(data);//自带换行
    cout<<strData;
#endif
    return strData;
}

#if 1
void parseJson1(const string & data)
{
    Json::Value root,word;
    Json::Reader reader;
    reader.parse(data,root);
    word=root["word"];
    cout<<"similar word:";
    for(unsigned int i=0;i<word.size();++i)
    {
        //string words="word"+string(1,'0'+i);
        //cout<<word[words].asString()<<" ";
        cout<<word[i].asString()<<" ";
        //cout<<word[i]<<" ";//不加asString，会自动换行
    }
    cout<<endl;
}
#endif

#if 0//1.8版json
bool parseJson(const string & data)
{
    if(data.empty())
        return false;
    bool res;
    JSONCPP_STRING errs;
    Json::Value root,word;
    Json::CharReaderBuilder reader;
    unique_ptr<Json::CharReader> const jsonreaer(reader.newCharReader());
    res=jsonreaer->parse(data.c_str(),data.c_str()+data.length(),&root,&errs);
    if(!res||!errs.empty()){
        cout<<"parseJosn error."<<errs<<endl;
    }
    word=root["word"];
    cout<<"similar word:";
    for(unsigned int i=0;i<word.size();++i)
    {
        cout<<word[i].asString()<<" ";//自动将中文转为utf-8输出
        //cout<<word[i]<<" ";
    }
    cout<<endl;
    return true;
}
#endif

int main()
{
    string word=createJson();
    //parseJson(word);
    parseJson1(word);
    return 0;
}

