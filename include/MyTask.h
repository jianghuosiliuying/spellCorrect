#pragma once

#include <string>
#include <queue>
#include <set>
#include <memory>

using namespace std;

namespace mm
{

class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

struct MyResult
{
    string word_;//候选词
    int iFreq_;//词频
    int iDist_;//与查询词的最小编辑距离
};

struct MyCompare
{
    bool operator()(const MyResult & lhs,const MyResult & rhs)
    {
        if(lhs.iDist_!=rhs.iDist_)
            return lhs.iDist_>rhs.iDist_;//编辑距离按升序
        else if(lhs.iFreq_!=rhs.iFreq_){
            return lhs.iFreq_<rhs.iFreq_;//词频降序
        }else{
            return lhs.word_>rhs.word_;//单词字母升序
        }
    }
};

class MyTask
{
public:
	MyTask(const string & msg,const TcpConnectionPtr & conn);

	//运行在线程池的某一个子线程中
	void process();
    //void execute(Cache & cache);//执行查询
    void queryIndexTable();//查询索引
    void statistic();//计算出下标对应的单词并存储
    int distance(const string & rhs);//计算最小编辑距离
    //void response(Cache & cache);//响应客户端请求
private:
	string _msg;//等待查询的单词
	TcpConnectionPtr _conn;
    set<int> location_;//与目标单词相关所有单词下标的集合
    vector<pair<string,int> > findset_;//与目标单词相关所有单词的集合
    priority_queue<MyResult,vector<MyResult>,MyCompare> resultQue_;//查询结果优先级队列
};

}//end of namespace mm
