#include "../common/common.h"
#include "../common/server.h"
#include "../hashTable/hashTable.h"




hashTable *hash;

typedef void (*signal_handler)(int);

int serverPort;


void heart_handler_fun(int signal_num);

int main(int argc,char *argv[]) 
{
    if (!argv[1]) {
        printf("must port!\n");
        mcLog("error.log", "must port");
        exit(1);
    }
    
    serverPort = atoi(argv[1]);
    
    int p = fork();
    if (p < 0) {
        printf("fork error!\n");
    }
    //child process 
    if (p == 0) {
        signal_handler signalFun = heart_handler_fun;
        signal(SIGALRM, signalFun);
        alarm(5);
        while(1){
            /* code */
        }
    }
    
    
    hash = hashInit(16);
    int listenfd = serverInit();
    epollEventLoop(listenfd);
    return 0;
}


void heart_handler_fun(int signal_num)
{
    alarm(5);
    mcLog("alarm.log","dingshiqi");
}


    




