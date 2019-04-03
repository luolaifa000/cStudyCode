#include "common.h"
#include "server.h"
#include "../hashTable/hashTable.h"


//指令回调
command_s command_arr[] = {
    {"run", 3, runCommandHandler,NULL},
    {"walk", 4, walkCommandHandler,NULL},
    {"set", 3, setCommandHandler,NULL},
    {"get", 3, getCommandHandler,NULL},
    {"", 0, NULL}
};


/**
 * 服务器启动初始化
 * 
*/
int serverInit()
{
    struct sockaddr_in servaddr;
    messageBox boxData;
    bzero(&servaddr, sizeof(servaddr));
    bzero(&boxData, sizeof(boxData));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    servaddr.sin_port = htons (SERVER_PORT);
    int listenfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
        printf("bind:%s\n",strerror(errno));
        return 1;
    }

    listen(listenfd, MAX_LISTEN_NUM);
    epfd = epoll_create(MAX_EPOLL_NUM); 

    epollEventDdl(EPOLL_CTL_ADD, listenfd, EPOLLIN|EPOLLET, NULL, NULL, NULL);
    return listenfd;
}

/**
 * epoll事件阻塞
 * 
*/
void epollEventLoop(int listenfd)
{
    struct epoll_event events[5]; 
    int i,n,ret;
    int socket;
    for(;;)
    {
        nfds = epoll_wait(epfd, events, 5, -1);
        for (i=0; i<nfds; i++)
        {
            messageBox *box = (messageBox *)(events[i].data.ptr);
            socket = box->socket;
            if (socket == listenfd && (events[i].events & EPOLLIN)) {
                handerAccept(listenfd);
            } else if(events[i].events & EPOLLOUT) {
                (*((epollHandle) box->writeHandle))(box);
            } else if(events[i].events & EPOLLIN) {
                (*((epollHandle) box->readHandle))(box);
            } 
        }
    }
}


/**
 * run命令回调
 * 
*/
void setCommandHandler(char *name)
{
    printf("%s\n", name);
}

/**
 * walk命令回调
 * 
*/
void getCommandHandler(char *name)
{
    printf("%s\n", name);
}


/**
 * run命令回调
 * 
*/
void runCommandHandler(char *name)
{
    printf("%s\n", name);
}

/**
 * walk命令回调
 * 
*/
void walkCommandHandler(char *name)
{
    printf("%s\n", name);
}

/**
 * 指定回调处理
 * 
*/
void handlerCommand(char *name)
{
    printf("name:%s\n",name);
    mcLog("demo.log",name);
    command_s *p = command_arr;

    int i = 0;
    char **argv = defMalloc(sizeof(void *) * 3);
    
    char *sss= defMalloc(sizeof(void *) * 1);
    while (*name) {
        if (*name != ' ') {
            *argv[i] = *name++;
        } else {
             i++;
        }
    }

    printf("i:%d\n",i);

    
    for ( ; p->len ; p++) {
        if (strcmp(name, p->name) == 0) {
            (*(commandHandlerPointer)(p->callback))(p->name);
        }
    }
}
/**
 * 读事件回调
 * 
*/
void readCallback(messageBox *box)
{
    int fd = box->socket;
    int nread;
    
    char *string = (char *)defMalloc(100);
    
    nread = read(fd,string, 100);
    if (nread == -1) {
        perror("read error\n");
    }
    if (nread == 0) {
        perror("close\n");
        close(fd);
    }

    printf("readdata:%s\n", string);
    
    handlerCommand(string);

    epollEventDdl(EPOLL_CTL_MOD, fd, EPOLLOUT|EPOLLET, readCallback, writeCallback, "send data gei ni");

    free(string);
}

/**
 * 注册修改删除事件
 **/
void epollEventDdl(int operation, int fd, int type, epollHandle rFunc,epollHandle wFunc, char *data)
{
    struct epoll_event ev;
    messageBox *boxdata;
    boxdata = defMalloc(sizeof(messageBox));
    ev.events = type;
    boxdata->socket = fd;
    boxdata->data = data;
    boxdata->readHandle = rFunc;
    boxdata->writeHandle = wFunc;
    ev.data.ptr = boxdata;
    epoll_ctl(epfd, operation, fd, &ev);
}

/**
 * 写事件回调
 **/
void writeCallback(messageBox *box)
{
    int fd = box->socket;
    int nwrite;
    nwrite = write(fd,box->data,strlen(box->data));
    if (nwrite == -1) {
        perror("write error\n");
    }
    if (nwrite > 0) {
        epollEventDdl(EPOLL_CTL_MOD, fd, EPOLLIN|EPOLLET, readCallback, writeCallback, NULL);
    }
}


/**
 * 客户端连接事件回调
 **/
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
    
    epollEventDdl(EPOLL_CTL_ADD, clientFd, EPOLLIN|EPOLLET, readCallback, writeCallback, NULL);
}







