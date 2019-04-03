#include "common/common.h"
#include "common/server.h"
#include "hashTable/hashTable.h"


int main(int argc,char *argv[]) 
{
    int listenfd = serverInit();
    epollEventLoop(listenfd);
    return 0;
}
    




