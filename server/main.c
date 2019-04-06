#include "../common/common.h"
#include "../common/server.h"
#include "../hashTable/hashTable.h"




hashTable *hash;

typedef void (*signal_handler)(int);

int serverPort;

int channel[2];

void childProcess();

void parentProcess();


void heart_handler_fun(int signal_num);

int main(int argc,char *argv[]) 
{

    


    if (!argv[1]) {
        printf("must port!\n");
        mcLog("error.log", "must port");
        exit(1);
    }

    deamonInit();
    
    serverPort = atoi(argv[1]);

    //parent 0 write 1 read
    //child  0 read  1 write
    int flag = socketpair(AF_UNIX, SOCK_STREAM, 0, channel);
    if (flag != 0) {
        printf("socketpair error!\n");
        exit(1);
    }

    int p = fork();
    if (p < 0) {
        printf("fork error!\n");
        exit(1);
    }

    //child process 
    if (p == 0) {
        //处理客户端的请求，响应心跳信息
        childProcess();
        printf("321");
    } else {    
        //parent process
        //定时向子进程发送心跳消息
        parentProcess();
        printf("123");
        
    }
    
    return 0;
}

void childProcess()
{
    close(channel[0]);

    hash = hashInit(16);

    int listenfd = serverInit();

    epfd = epollEventInit();

    //将监听的端口的描述符添加事件树中
    epollEventDdl(epfd, EPOLL_CTL_ADD, listenfd, EPOLLIN|EPOLLET, NULL, NULL, NULL);

    //将监听的SOCKETPAIRE描述符添加事件树中
    epollEventDdl(epfd, EPOLL_CTL_ADD,  channel[1], EPOLLIN|EPOLLET, NULL, NULL, NULL);

    epollEventLoop(listenfd, epfd);
}


void parentProcess()
{
    close(channel[1]);

    signal_handler signalFun = heart_handler_fun;
    signal(SIGALRM, signalFun);
    alarm(5);
    
    epfdP = epollEventInit();

    epollEventDdl(epfdP, EPOLL_CTL_ADD,  channel[0], EPOLLIN|EPOLLET, NULL, NULL, NULL);

    epollEventLoop(0, epfdP);
}


void heart_handler_fun(int signal_num)
{
    printf("dingshiqi\n");
    mcLog("alarm.log","dingshiqi");
    char *heartString = (char *) malloc(100);
    sprintf(heartString, "pid:%d,i am parent!",getpid());
    write(channel[0], heartString, strlen(heartString));
    free(heartString);
    alarm(5);
}


    




