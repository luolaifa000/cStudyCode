#include "common/common.h"
#include "common/server.h"
#include "hashTable/hashTable.h"

hashTable *hash;



int main(int argc,char *argv[]) 
{
    //handlerCommand("get name");
    //return 0;
    hash = hashInit(16);
    int listenfd = serverInit();
    epollEventLoop(listenfd);
    return 0;
}
    




