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

int main(int argc, const char *argv[])
{
    int peerfd = socket(PF_INET, SOCK_STREAM, 0);
    if(peerfd == -1)
        ERR_EXIT("socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.3.204"); //localhost
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

bool parseJson(const string & data,char * word)
{
    if(data.empty())
        return false;
    bool res;
    JSONCPP_STRING errs;
    Json::Value root,info;
    Json::CharReaderBuilder reader;
    unique_ptr<Json::CharReader> const jsonreaer(reader.newCharReader());
    res=jsonreaer->parse(data.c_str(),data.c_str()+data.length(),&root,&errs);
    if(!res||!errs.empty()){
        cout<<"parseJosn error."<<errs<<endl;
    }
    info=root[word];
    cout<<"similar word:";
    for(unsigned int i=0;i<info.size();++i)
    {
        cout<<info[i]<<" ";
    }
    cout<<endl;
    return true;
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
        char word[4096]={0};
        int k=0;
        for(int i=0;i<(int)strlen(sendbuf);++i)
        {
            if((sendbuf[i]>='a'&&sendbuf[i]<='z')||(sendbuf[i]>='A'&&sendbuf[i]<='Z'))
                word[k++]=sendbuf[i];
        }
        int len=strlen(word);
        if(0==len)
            goto fgets;
        train.dataLen=strlen(word);
        strcpy(train.buf,word);
        write(sockfd, &train, 4+train.dataLen);//发送单词
		//sleep(5);

        //read
        recv(sockfd,&dataLen,4,0);
        cout<<"dataLen="<<dataLen<<endl;
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

