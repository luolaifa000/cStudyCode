#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#include "common/common.h"


int epfd,nfds;
char buf[MAX_SIZE];
void handerAccept(int listenfd);
void addEvent(int fd, int state);

void readData(int fd);
void writeData(int fd);
void modifyEvent(int fd, int state);


int main(int argc,char *argv[]) 
{
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8787);
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    int i = connect(sockfd, (struct sockaddr*)&servaddr,sizeof(servaddr));
    if (i != 0) {
        perror("connect error!\n");
        return 1;
    }
    
    struct epoll_event ev,events[5]; 
    epfd = epoll_create(MAX_EPOLL_NUM); 
    ev.events = EPOLLOUT|EPOLLET;
    ev.data.fd = sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    
    for (;;)
    {
        nfds = epoll_wait(epfd, events, 5, -1);
        
        for (i=0; i<nfds; i++)
        {
            if(events[i].events == EPOLLOUT) {
                writeData(events[i].data.fd);
                modifyEvent(events[i].data.fd, EPOLLIN|EPOLLET);
            } else if(events[i].events == EPOLLIN){
                readData(events[i].data.fd);
                modifyEvent(events[i].data.fd, EPOLLOUT|EPOLLET);
            } 
        }
       
    }
    return 0;
}


void writeData(int fd)
{
 
    int nwrite;
    char tempBuf[100];
 
    printf("请输入指令:");
    scanf("%s", tempBuf);


    nwrite = write(fd,tempBuf,strlen(tempBuf));
    if (nwrite == -1) {
        perror("write error\n");
    }
}
void readData(int fd)
{
    int nread;
    nread = read(fd,buf,MAX_SIZE);
    if (nread == -1) {
        perror("read error\n");
    }

    printf("readdata:%s\n", buf);
}

void handerAccept(int listenfd)
{
    int clientFd;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;
    clientFd = accept(listenfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientFd == -1) {
        perror("error");
    }
    printf("accept a new client: %s:%d\n",inet_ntoa(clientAddr.sin_addr),clientAddr.sin_port);

    addEvent(clientFd, EPOLLIN|EPOLLET);
}


void addEvent(int fd, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}


void modifyEvent(int fd, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
}