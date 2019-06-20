#include <iostream>
#include <vector>
#include <queue>
#include <string>

using namespace std;


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
            return lhs.iDist_>rhs.iDist_;
        else if(lhs.iFreq_!=rhs.iFreq_){
            return lhs.iFreq_<rhs.iFreq_;
        }else{
            return lhs.word_>rhs.word_;
        }
    }
};

int distance(const string & word,const string & word2)//计算最小编辑距离
{
    string word1=word;
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

void test(const string word)
{
    vector<pair<string,int>> wordcnt;
    wordcnt.push_back(make_pair("ros",48));
    wordcnt.push_back(make_pair("hros",68));
    wordcnt.push_back(make_pair("rsos",88));
    wordcnt.push_back(make_pair("rosd",28));
    wordcnt.push_back(make_pair("road",28));
    wordcnt.push_back(make_pair("rosa",28));
    MyResult result;
    priority_queue<MyResult,vector<MyResult>,MyCompare> que;
    for(auto & k:wordcnt)
    {
        result.word_=k.first;
        result.iFreq_=k.second;
        result.iDist_=distance(word,k.first);
        que.push(result);
    }
    while(!que.empty())
    {
        cout<<que.top().word_<<" "<<que.top().iFreq_<<" "<<que.top().iDist_<<endl;
        que.pop();
    }
}

int main()
{
    string word("horse");
    test(word);
    return 0;
}

