typedef struct {
    int socket;
    void *data;
} messageBox;

int epfd,nfds;

int clientInit();

void epollEventLoop();

void epollEventDdl(int operation, int fd, int type, char *data);

void writeData(int fd);



