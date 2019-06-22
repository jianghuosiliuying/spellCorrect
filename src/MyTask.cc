#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif
#include "../include/MyTask.h"
#include "../include/TcpConnection.h"
#include "../include/Mydict.h"
#include "../include/json/json.h"
#include <iostream>
#include <map>
#include <vector>

using namespace std;

namespace mm
{
MyTask::MyTask(const string & msg,const mm::TcpConnectionPtr & conn)
: _msg(msg)//待查询单词
, _conn(conn)
{}

//运行在线程池的某一个子线程中
void MyTask::process()
{
	//decode
    queryIndexTable();
    statistic();
	//compute
    MyResult result;
    for(auto & word_cnt:findset_)
    {
        result.word_=word_cnt.first;
        result.iFreq_=word_cnt.second;
        result.iDist_=distance(word_cnt.first);
        resultQue_.push(result);
    }
	//encode
	string response;
    createJson(response);//要返回给客户端的消息
	//string response = _msg;//要返回给客户端的消息
	//_conn->send(response);//由线程池的线程(计算线程)完成数据的发送,在设计上来说，是不合理的
						  //数据发送的工作要交还给IO线程(Reactor所在的线程)完成
						  //将send的函数的执行延迟到IO线程取操作
	_conn->sendInLoop(response);
}
//void MyTask::execute(Cache & cache);//执行查询
void MyTask::createJson(string & response)//建立json
{
    Json::Value root;
    Json::Value data;
    for(int i=0;!resultQue_.empty() && i<5;++i)
    {//建立json数组
        data[i]=resultQue_.top().word_;
        resultQue_.pop();
    }
    root[_msg]=data;
    Json::FastWriter writer;
    response=writer.write(root);
    //ostringstream os;
    //Json::StreamWriterBuilder writer;
    //unique_ptr<Json::StreamWriter> jsonwriter(writer.newStreamWriter());
    //jsonwriter->write(root,&os);
    //response=os.str();
    cout<<response<<endl;
}
void MyTask::queryIndexTable()//查询索引
{
    Mydict * pmydict=Mydict::createMydict();//
    map<string,set<int> > index=pmydict->getIndexTable();//
    set<char> tmp(_msg.begin(),_msg.end());
    string word;
    for(auto & c:tmp)
    {
        word+=c;
    }//去重单词
    for(auto & ch:word)
    {
        auto it=index.find(string(1,ch));
        if(it!=index.end())
        {
            for(auto & i:it->second)
            {
                location_.insert(i);
            }
        }
    }
}
void MyTask::statistic()//计算出下标对应的单词并存储
{
    Mydict * pmydict=Mydict::createMydict();//
    vector<pair<string,int> > dict=pmydict->getDict();//
    for(auto & i:location_)
    {
        findset_.push_back(dict[i]);
    }
}
int MyTask::distance(const string & word2)//计算最小编辑距离
{
    string word1=_msg;
    int m=word1.size();
    int n=word2.size();
    for(int k=0;k!=m;++k)
    {
        word1[k]=tolower(word1[k]);
    }
    if(m*n==0)  return m+n;
    vector<vector<int> > d(m+1,vector<int>(n+1));
    for(int i=0;i<=m;++i){  d[i][0]=i;  }
    for(int j=0;j<=n;++j){  d[0][j]=j;  }
    for(int i=1;i<=m;++i)
    {
        for(int j=1;j<=n;++j)
        {
            if(word1[i-1]==word2[j-1]){
                d[i][j]=d[i-1][j-1];
            }else{
                d[i][j]=1+min(d[i-1][j-1],min(d[i-1][j],d[i][j-1]));
            }
        }
    }
    return d[m][n];
}
//void MyTask::response(Cache & cache)//响应客户端请求
//{}

}//end of namespace mm
