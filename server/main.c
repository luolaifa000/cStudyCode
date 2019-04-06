#include "../common/common.h"
#include "../common/server.h"
#include "../hashTable/hashTable.h"




hashTable *hash;

typedef void (*signal_handler)(int);

int serverPort;

int channel[2];

void childProcess();


void heart_handler_fun(int signal_num);

int main(int argc,char *argv[]) 
{
    if (!argv[1]) {
        printf("must port!\n");
        mcLog("error.log", "must port");
        exit(1);
    }
    
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
        close(channel[0]);
        childProcess();
    } else {    
        //parent process
        //定时向子进程发送心跳消息
        close(channel[1]);
        signal_handler signalFun = heart_handler_fun;
        signal(SIGALRM, signalFun);
        alarm(5);
        hash = hashInit(16);
        int listenfd = serverInit();

        epollEventDdl(EPOLL_CTL_ADD,  channel[0], EPOLLIN|EPOLLET, NULL, NULL, NULL);

        epollEventLoop(listenfd);
        return 0;
    }
    
    
}


void heart_handler_fun(int signal_num)
{
    mcLog("alarm.log","dingshiqi");
    char *heartString = (char *) malloc(100);
    sprintf(heartString, "pid:%d,i am parent!",getpid());
    write(channel[0], heartString, strlen(heartString));
    free(heartString);
    alarm(5);
}


void childProcess()
{
    epfdChild = epoll_create(MAX_EPOLL_NUM); 
    struct epoll_event ev;
    ev.events = EPOLLIN|EPOLLET;
    ev.data.fd = channel[1];
    epoll_ctl(epfdChild, EPOLL_CTL_ADD, channel[1], &ev);

    struct epoll_event events[5]; 
    int i,n,ret;
    int socket;
    for(;;)
    {
        nfds = epoll_wait(epfdChild, events, 5, -1);
        
        for (i=0; i<nfds; i++)
        {
            socket = events[i].data.fd;
            if(events[i].events & EPOLLIN) {
                char *rec = (char *) malloc(100);
                memset(rec,0,100);
                read(socket,rec, 100);
                //printf("rec:%s\n", rec);
                mcLog("child_heart.log",rec);
                memset(rec,0,100);
                sprintf(rec, "pid:%d,i am child!",getpid());
                write(socket, rec, strlen(rec));
                free(rec);
            } 
        }
    }
    exit(0);
}


    




