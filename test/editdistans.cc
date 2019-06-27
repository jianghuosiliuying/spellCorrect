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
int nBytesCode(const char ch)
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
int length(const string & str)
{//计算词语的长度
    int len=0;
    //cout<<"sz="<<str.size()<<endl;
    for(size_t i=0;i!=str.size();++i)
    {
        int nBytes=nBytesCode(str[i]);
        //cout<<nBytes<<endl;
        i+=(nBytes-1);
        ++len;
    }
    return len;
}

int distance(string & word1,const string & word2)//计算最小编辑距离
{
    int m=length(word1);//客户端传来的单词或词语
    int n=length(word2);//词典中的单词或词语
    //cout<<"m="<<m<<" n="<<n<<endl;
    vector<string> words1;//将每个字符或汉字存入数组
    vector<string> words2;//将每个字符或汉字存入数组
    size_t cur=0;
    while(cur!=word1.size())
    {//如果是单词，则转为小写
        int n=nBytesCode(word1[cur]);
        if(n==1)
            word1[cur]=tolower(word1[cur]);
        string ch=word1.substr(cur,n);
        //cout<<ch<<" ";
        words1.push_back(ch);//将Word1存到数组中
        cur+=n;
    }
    cur=0;
    while(cur!=word2.size())
    {
        int n=nBytesCode(word2[cur]);
        string ch=word2.substr(cur,n);
        //cout<<ch<<" ";
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

void test(string word)
{
    vector<pair<string,int>> wordcnt;
    wordcnt.push_back(make_pair("ros",48));
    wordcnt.push_back(make_pair("hros",68));
    wordcnt.push_back(make_pair("rsos",88));
    wordcnt.push_back(make_pair("rosd",28));
    wordcnt.push_back(make_pair("你好",28));
    wordcnt.push_back(make_pair("我不好",28));
    wordcnt.push_back(make_pair("你不好",38));
    wordcnt.push_back(make_pair("一点",38));
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
    //string word("horse");
    string word("你好");
    test(word);
    return 0;
}

