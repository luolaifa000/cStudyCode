
#ifndef COMMON_HEADER
#define COMMON_HEADER 1
#include "common/common.h"
#endif

#ifndef SERVER_HEADER
#define SERVER_HEADER 1
#include "common/server.h"
#endif


typedef struct {
    int socket;
    void *data;
    void *readHandle;
    void *writeHandle;
} box;


typedef struct {
    char *name;
    int len;
    void *callback;
} command_s;
typedef void (*epollHandle)(box*);

typedef void (*commandHandlerPointer)(char*);

int epfd,nfds;

void handerAccept(int listenfd);
void readCallback(box *readBox);
void writeCallback(box *readBox);
void handlerCommand(char *name);
void runCommandHandler(char *name);
void walkCommandHandler(char *name);
void mcLog(char *string);


//指令回调
command_s command_arr[] = {
    {"run", 3, runCommandHandler},
    {"walk", 4, walkCommandHandler},
    {"", 0, NULL}
};



int main(int argc,char *argv[]) 
{
    test(2);
    struct sockaddr_in cliaddr, servaddr;
    box boxData;
    bzero(&servaddr, sizeof(servaddr));
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
                box *writeBox = (box *)(events[i].data.ptr);
                (*((epollHandle) writeBox->writeHandle))(writeBox);

            } else if(events[i].events == EPOLLIN){
                box *readBox = (box *)(events[i].data.ptr);
                (*((epollHandle) readBox->readHandle))(readBox);
            } 
        }
    }
    return 0;
}


void runCommandHandler(char *name)
{
    mcLog(name);
    printf("%s\n", name);
}

void walkCommandHandler(char *name)
{
    mcLog(name);
    printf("%s\n", name);
}

/*
    指定回调处理
*/
void handlerCommand(char *name)
{
    command_s *p = command_arr;
    
    for ( ; p->len ; p++) {
        if (strcmp(name, p->name) == 0) {
            (*(commandHandlerPointer)(p->callback))(p->name);
        }
    }
}

void readCallback(box *readBox)
{
    int fd = readBox->socket;
    int nread;
    char *string = (char *) malloc(15);
    char ss[100];
    nread = read(fd,ss,100);
    if (nread == -1) {
        perror("read error\n");
    }
    if (nread == 0) {
        perror("close\n");
        close(fd);
    }

    printf("readdata:%s\n", ss);
    
    handlerCommand(ss);
    struct epoll_event ev;
    box boxdata;

    ev.events = EPOLLOUT|EPOLLET;
    ev.data.fd = fd;
    boxdata.socket = fd;
    boxdata.data = "iam box";
    boxdata.writeHandle = writeCallback;
    ev.data.ptr = &boxdata;
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
    free(string);
}


void writeCallback(box *readBox)
{
    int fd = readBox->socket;
    int nwrite;
    char *tempBuf = "i am server";
    nwrite = write(fd,tempBuf,strlen(tempBuf));
    if (nwrite == -1) {
        perror("write error\n");
    }
    if (nwrite > 0) {
        struct epoll_event ev;
        box boxdata;
        ev.events = EPOLLIN|EPOLLET;
        ev.data.fd = fd;
        boxdata.socket = fd;
        boxdata.data = "iam box";
        boxdata.readHandle = readCallback;
        ev.data.ptr = &boxdata;
        epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
    }
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

    struct epoll_event ev;
    box boxdata;
    ev.events = EPOLLIN|EPOLLET;
    ev.data.fd = clientFd;
    boxdata.socket = clientFd;
    boxdata.data = "iam box";
    boxdata.readHandle = readCallback;
    ev.data.ptr = &boxdata;
    epoll_ctl(epfd, EPOLL_CTL_ADD, clientFd, &ev);
}



void mcLog(char *string)
{
    int fd;
    char *s = (char *) malloc(strlen(string) + 2);
    char *ss = s;
    while(*string) {
        *s = *string;
        s++;
        string++;
    }
    *s = '\n';
    fd = open("epoll.log",O_RDWR|O_CREAT|O_APPEND, "0777");
    write(fd, ss, strlen(ss));
    close(fd);
}




