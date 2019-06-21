#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
	read(peerfd, buf, sizeof(buf));
	printf("%s\n", buf);

    do_service(peerfd);
    return 0;
}

void do_service(int sockfd)
{
    char recvbuf[4096] = {0};
    char sendbuf[4096] = {0};
    while(1)
    {
        struct Train train;
        //scanf("%s",sendbuf);
        //printf("%s\n",sendbuf);
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
        write(sockfd, &train, 4+train.dataLen);
		//sleep(5);

        //read
        int dataLen;
        recv(sockfd,&dataLen,4,0);
        int nread = read(sockfd, recvbuf, dataLen);
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

        printf("receive msg : %s\n", recvbuf);

        memset(recvbuf, 0, sizeof recvbuf);
        memset(sendbuf, 0, sizeof sendbuf);
    }
}

