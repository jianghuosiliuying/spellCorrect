#include "json.h"
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
    Json::StreamWriterBuilder writer;
    unique_ptr<Json::StreamWriter> jsonwriter(writer.newStreamWriter());
    jsonwriter->write(root,&os);
    string strData=os.str();
    //cout<<root<<endl;
    //cout<<data<<endl;
    cout<<strData<<endl;
    return strData;
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
    return 0;
}

