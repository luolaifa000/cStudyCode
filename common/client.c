#include "common.h"
#include "client.h"
#include "../hashTable/hashTable.h"


/**
 * 服务器启动初始化
 * 
*/
int clientInit()
{
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET,SERVER_IP,&servaddr.sin_addr);
    int i = connect(sockfd, (struct sockaddr*)&servaddr,sizeof(servaddr));
    if (i != 0) {
        perror("connect error!\n");
        return 1;
    }
    epfd = epoll_create(MAX_EPOLL_NUM); 

    epollEventDdl(EPOLL_CTL_ADD, sockfd, EPOLLOUT|EPOLLET,  NULL);
    return sockfd;
}


/**
 * epoll事件阻塞
 * 
*/
void epollEventLoop()
{
    struct epoll_event events[5]; 
    int i,n,ret;
    int socket;
    for (;;)
    {
        nfds = epoll_wait(epfd, events, 5, -1);
        
        for (i=0; i<nfds; i++)
        {
            messageBox *box = (messageBox *)(events[i].data.ptr);
            socket = box->socket;

            if(events[i].events & EPOLLOUT) {
                writeData(socket);
                epollEventDdl(EPOLL_CTL_MOD, socket, EPOLLIN|EPOLLET, NULL);
            } else if(events[i].events & EPOLLIN){
                char *string = (char *)defMalloc(100);
    
                ret = read(socket,string, 100);
                if (ret == -1) {
                    perror("read error\n");
                }
                if (ret == 0) {
                    perror("close\n");
                    close(socket);
                }

                printf("readdata:%s\n", string);
                 epollEventDdl(EPOLL_CTL_MOD, socket, EPOLLOUT|EPOLLET, NULL);
            } 
        }
       
    }
}




/**
 * 注册修改删除事件
 **/
void epollEventDdl(int operation, int fd, int type, char *data)
{
    struct epoll_event ev;
    messageBox *boxdata = (messageBox *) defMalloc(sizeof(messageBox));
    ev.events = type;
    boxdata->socket = fd;
    boxdata->data = data;
    ev.data.ptr = boxdata;
    epoll_ctl(epfd, operation, fd, &ev);
}



void writeData(int fd)
{
 
    int nwrite;
    char tempBuf[100];
 
    printf("请输入指令:");
    fgets(tempBuf, sizeof(tempBuf), stdin);


    nwrite = write(fd,tempBuf,strlen(tempBuf));
    if (nwrite == -1) {
        perror("write error\n");
    }
}











