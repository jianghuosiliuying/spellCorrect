#include "../include/json/json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <memory>

using namespace std;

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

struct Train
{
    int dataLen;
    char buf[4096];
};

void do_service(int sockfd);

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

int main(int argc, const char *argv[])
{
    int peerfd = socket(PF_INET, SOCK_STREAM, 0);
    if(peerfd == -1)
        ERR_EXIT("socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.80.128"); //localhost
    //addr.sin_addr.s_addr = INADDR_ANY; //localhost
    addr.sin_port = htons(2000);
    socklen_t len = sizeof addr;
    if(connect(peerfd, (struct sockaddr*)&addr, len) == -1)
        ERR_EXIT("Connect");

	char buf[1024];
	memset(buf, 0, sizeof(buf));
        int dataLen;
        int nread;
        recv(peerfd,&dataLen,4,0);
        nread = read(peerfd, buf, dataLen);
	//read(peerfd, buf, sizeof(buf));
	printf("%s\n", buf);

    do_service(peerfd);
    return 0;
}

void parseJson(const string & data,string words)
{
    Json::Value root,word;
    Json::Reader reader;
    reader.parse(data,root);
    word=root[words];
    cout<<"similar word --->";
    for(unsigned int i=0;i<word.size();++i)
    {
        cout<<word[i].asString()<<" ";
    }
    cout<<endl;
}

void do_service(int sockfd)
{
    char recvbuf[4096] = {0};
    char sendbuf[4096] = {0};
    while(1)
    {
        int dataLen;
        int nread;
        struct Train train;
    fgets:
        fgets(sendbuf, sizeof sendbuf, stdin);
        //char word[4096]={0};
        string word;
        size_t cur=0;
        string wordString(sendbuf);
        while(cur!=wordString.size())
        {//如果是单词，则去掉其中非字母部分
            int n=nBytesCode(wordString[cur]);
            string ch=wordString.substr(cur,n);
            if(n==1 && isalpha(wordString[cur])){
                word.append(ch);
            }else if(n>1){//如果是汉字或汉字字符，则加入word
                word.append(ch);
            }
            cur+=n;
        }
        int len=word.size();
        if(0==len)
            goto fgets;
        train.dataLen=word.size();
        strcpy(train.buf,word.c_str());
        write(sockfd, &train, 4+train.dataLen);//发送单词
		//sleep(5);

        //read
        recv(sockfd,&dataLen,4,0);
        //cout<<"dataLen="<<dataLen<<endl;
        nread = read(sockfd, recvbuf, dataLen);
        //cout<<"recvbuf="<<recvbuf<<" nread="<<nread<<endl;
        if(nread == -1)
        {
            if(errno == EINTR)
                continue;
            ERR_EXIT("read");
        }
        else if(nread == 0) //代表链接断开
        {
            printf("server close!\n");
            close(sockfd);
            exit(EXIT_SUCCESS);
        }
        
        string data(recvbuf);
        parseJson(data,word);//解析json
        //printf("receive msg : %s\n", recvbuf);

        memset(recvbuf, 0, sizeof recvbuf);
        memset(sendbuf, 0, sizeof sendbuf);
    }
}

