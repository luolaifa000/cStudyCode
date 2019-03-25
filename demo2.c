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
#include <string.h>

#define MAX_LISTEN_NUM 5
#define MAX_EPOLL_NUM 20
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8787
#define MAX_SIZE 1000




typedef struct {
    int socket;
    char *data;
    void *readHandle;
    void *writeHandle;
} box;


typedef void (*epollHandle)(box*);



int epfd,nfds;
char buf[MAX_SIZE];
void handerAccept(int listenfd);

void readCallback(box *box);
void writeCallback(int fd);






int main(int argc,char *argv[]) 
{
    struct sockaddr_in cliaddr, servaddr;
    box boxData;
    bzero(&servaddr, sizeof(servaddr));
    bzero(&buf, sizeof(MAX_SIZE));
    bzero(&boxData, sizeof(boxData));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    servaddr.sin_port = htons (SERVER_PORT);
    int listenfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
        perror("bind error!");
        return 1;
    }
    listen(listenfd, MAX_LISTEN_NUM);
    struct epoll_event ev,events[5]; 
    epfd = epoll_create(MAX_EPOLL_NUM); 
    ev.events = EPOLLIN|EPOLLET;
    ev.data.fd = listenfd;
    
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

    int i,n,ret;
    int socket;
    
    for(;;)
    {
        nfds = epoll_wait(epfd, events, 5, -1);
        for (i=0; i<nfds; i++)
        {
            socket = events[i].data.fd;
            if (socket == listenfd && events[i].events & EPOLLIN) {
                handerAccept(listenfd);
            } else if(events[i].events == EPOLLOUT) {

            } else if(events[i].events == EPOLLIN){
                box *aaa = (box *)(events[i].data.ptr);
                epollHandle func = (epollHandle) aaa->readHandle;
                func(aaa);
            } 
        }
    }

    return 0;
    
}

void readCallback(box *box)
{
    int fd = box->socket;
    int nread;
    bzero(&buf, sizeof(MAX_SIZE));
    nread = read(fd,buf,MAX_SIZE);
    if (nread == -1) {
        perror("read error\n");
    }
    if (nread == 0) {
        perror("close\n");
        close(fd);
    }

    printf("readdata:%s\n", buf);
    int ret;
    ret = strncmp(buf, "i am client", 11);
    if (ret == 0) {
        printf("set\n");
    }

    struct epoll_event ev;
    ev.events = EPOLLOUT|EPOLLET;
    ev.data.fd = fd;
    //ev.data.ptr = writeCallback;
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
}


void writeCallback(int fd)
{
    int nwrite;
    char tempBuf[100] = "i am server";
    nwrite = write(fd,tempBuf,strlen(tempBuf));
    if (nwrite == -1) {
        perror("write error\n");
    }
    if (nwrite > 0) {
        struct epoll_event ev;
        ev.events = EPOLLIN|EPOLLET;
        ev.data.fd = fd;
        ev.data.ptr = readCallback;
        epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
    }
    
}




void handerAccept(int listenfd)
{
    int clientFd;
    struct sockaddr_in clientAddr;
    box boxdata;
    socklen_t clientAddrLen;
    clientFd = accept(listenfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientFd == -1) {
        perror("error");
    }
    printf("accept a new client: %s:%d\n",inet_ntoa(clientAddr.sin_addr),clientAddr.sin_port);

    struct epoll_event ev;
    ev.events = EPOLLIN|EPOLLET;
    ev.data.fd = clientFd;
    boxdata.socket = clientFd;
    boxdata.data = "iam box";
    boxdata.readHandle = readCallback;
    ev.data.ptr = &boxdata;
    epoll_ctl(epfd, EPOLL_CTL_ADD, clientFd, &ev);
}


