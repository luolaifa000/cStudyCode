#include "common/common.h"
#include "common/server.h"
#include "hashTable/hashTable.h"


int main(int argc,char *argv[]) 
{
    handlerCommand("set name luosdfsdf");
    return 0;
    int listenfd = serverInit();
    epollEventLoop(listenfd);
    return 0;
}
    




