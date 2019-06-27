#include "../include/MyTask.h"
#include "../include/TcpConnection.h"
#include "../include/Mydict.h"
#include "../include/json/json.h"
#include "../include/Thread.h"
#include "../include/CacheManger.h"
#include <iostream>
#include <map>
#include <vector>

using namespace std;

namespace mm
{
MyTask::MyTask(const string & msg,const mm::TcpConnectionPtr & conn)
: _msg(msg)//待查询单词
, _conn(conn)
,pcacheM_(CacheManger::createCacheManger())//拿到cacheManger对象指针
{}

//运行在线程池的某一个子线程中
void MyTask::process()
{
	string response;
    cout<<"i am "<<threadNum<<" thread, id="<<pthread_self()<<endl;
	//decode
    Cache & hotcache=pcacheM_->getCache(threadNum);
    string  hotdataJson=hotcache.searchElement(_msg);//在子线程热数据cache中查找
    if(!hotdataJson.empty())
    {//热数据命中
        cout<<threadNum<<" thread hotdata is hit."<<endl;
        response=hotdataJson;
    }else{//热数据未命中
        string retjson=pcacheM_->getCache(0).searchElement(_msg);
        if(!retjson.empty()){//主缓存命中
            cout<<threadNum<<" thread hotdata is not hit. main cache is hit."<<endl;
            response=retjson;
            //cout<<response<<endl;
        }else{//缓存未命中
            cout<<threadNum<<" thread hotdata is not hit. main cache is not hit, will compute."<<endl;
            queryIndexTable();//查找索引存到set
            statistic();//用set查找单词，存到vector中
	        //compute
            MyResult result;
            for(auto & word_cnt:findset_)//遍历vector并计算编辑距离，存到优先级队列里
            {
                result.word_=word_cnt.first;
                result.iFreq_=word_cnt.second;
                result.iDist_=distance(word_cnt.first);
                resultQue_.push(result);
            }
	        //encode
            createJson(response);//要返回给客户端的消息
            //pcacheM_->getCache(threadNum).getCacheMap().insert(make_pair(_msg,response));
            hotcache.addElement(_msg,response);//添加热数据
        }
    }
	//string response = _msg;//要返回给客户端的消息
	//_conn->send(response);//由线程池的线程(计算线程)完成数据的发送,在设计上来说，是不合理的
						  //数据发送的工作要交还给IO线程(Reactor所在的线程)完成
						  //将send的函数的执行延迟到IO线程取操作
    cout<<response<<endl;
	_conn->sendInLoop(response);
}
int MyTask::length(const string & str)
{//计算词语的长度
    int len=0;
    for(size_t i=0;i!=str.size();++i)
    {
        int nBytes=nBytesCode(str[i]);
        i+=(nBytes-1);
        ++len;
    }
    return len;
}
void MyTask::createJson(string & response)//建立json
{
    Json::Value data;
    for(int i=0;!resultQue_.empty() && i<5;++i)
    {//建立json数组
        data[_msg].append(resultQue_.top().word_);
        resultQue_.pop();
    }
    Json::FastWriter writer;
    response=writer.write(data);//fastwriter自带换行
}
int MyTask::nBytesCode(const char ch)
{//计算每个字的长度
    if(ch & (1<<7))
    {
        int nBytes=1;
        for(int i=0;i!=6;++i)
        {//utf-8最大占6个字节
            if(ch & (1<<(6-i))){
                ++nBytes;
            }else{
                break;
            }
        }
        return nBytes;
    }
    return 1;
}
void MyTask::queryIndexTable()//查询索引
{
    Mydict * pmydict=Mydict::createMydict();//创建词典单例
    map<string,set<int> > index=pmydict->getIndexTable();//拿到索引表
    //set<char> tmp(_msg.begin(),_msg.end());
    set<string> tmp;
    size_t cur=0;
    while(cur!=_msg.size())
    {//去重单词和汉字
        int nBytes=nBytesCode(_msg[cur]);
        string ch=_msg.substr(cur,nBytes);
        cur+=nBytes;
        tmp.insert(ch);
    }
    for(auto iter=tmp.begin();iter!=tmp.end();++iter)
    {//遍历set查询每个ch对应行号
        string ch=*iter;
        auto it=index.find(ch);
        if(it!=index.end())
        {
            for(auto & i:it->second)
            {//单词每个字母或汉字对应的下标集合
                location_.insert(i);
            }
        }//针对未出现过的汉字如何处理？
    }
}
void MyTask::statistic()//计算出下标对应的单词并存储
{
    Mydict * pmydict=Mydict::createMydict();//
    vector<pair<string,int> > dict=pmydict->getDict();//拿到字典的vector
    for(auto & i:location_)
    {
        findset_.push_back(dict[i]);
    }
}
int MyTask::distance(const string & word2)//计算最小编辑距离
{//修改为中英文都适用
    string word1=_msg;
    int m=length(word1);//客户端传来的单词或词语
    int n=length(word2);//词典中的单词或词语
    vector<string> words1;//将每个字符或汉字存入数组
    vector<string> words2;//将每个字符或汉字存入数组
    size_t cur=0;
    while(cur!=word1.size())
    {//如果是单词，则转为小写
        int n=nBytesCode(word1[cur]);
        if(n==1)
            word1[cur]=tolower(word1[cur]);
        string ch=word1.substr(cur,n);
        words1.push_back(ch);//将Word1存到数组中
        cur+=n;
    }
    cur=0;
    while(cur!=word2.size())
    {
        int n=nBytesCode(word2[cur]);
        string ch=word2.substr(cur,n);
        words2.push_back(ch);//将Word2存到数组中
        cur+=n;
    }
    if(m*n==0)  return m+n;
    vector<vector<int> > d(m+1,vector<int>(n+1));
    for(int i=0;i<=m;++i){  d[i][0]=i;  }
    for(int j=0;j<=n;++j){  d[0][j]=j;  }
    for(int i=1;i<=m;++i)
    {
        for(int j=1;j<=n;++j)
        {
            if(words1[i-1]==words2[j-1]){
                d[i][j]=d[i-1][j-1];
            }else{
                d[i][j]=1+min(d[i-1][j-1],min(d[i-1][j],d[i][j-1]));
            }
        }
    }
    return d[m][n];
}

}//end of namespace mm
