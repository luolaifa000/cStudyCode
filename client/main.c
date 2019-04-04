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

#include "../common/common.h"
#include "../common/client.h"




int main(int argc,char *argv[]) 
{
    
    clientInit();
    epollEventLoop();
    
}


