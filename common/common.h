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
#include <string.h>

#define MAX_LISTEN_NUM 5
#define MAX_EPOLL_NUM 20
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8787
#define MAX_SIZE 1000






