#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif
#include "../../include/json/json.h"
#include <iostream>
#include <string>
#include <memory>

using namespace std;

string createJson()
{
    Json::Value root;
    Json::Value data;
    ostringstream os;
    data[0]="hello";
    data[1]="world";
    data[2]="fine";
    root["word"]=data;
#if 0
    Json::StreamWriterBuilder writer;
    unique_ptr<Json::StreamWriter> jsonwriter(writer.newStreamWriter());
    jsonwriter->write(root,&os);
#endif
    Json::FastWriter writer;
    string strData=writer.write(root);//自带换行
    //string strData=os.str();
    cout<<strData;
    const char *str=strData.c_str();
    string str1(str);
    //cout<<strData<<endl;
    return strData;
}

void parseJson1(const string & data)
{
    Json::Value root,word;
    Json::Reader reader;
    reader.parse(data,root);
    word=root["word"];
    cout<<"similar word:";
    for(unsigned int i=0;i<word.size();++i)
    {
        cout<<word[i]<<" ";
    }
    cout<<endl;
}

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
        cout<<word[i]<<" ";
    }
    cout<<endl;
    return true;
}

int main()
{
    string word=createJson();
    parseJson(word);
    //parseJson1(word);
    return 0;
}

