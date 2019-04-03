

typedef struct {
    int socket;
    void *data;
    void *readHandle;
    void *writeHandle;
} messageBox;


typedef struct {
    char *name;
    int len;
    void *callback;
    void *value;
} command_s;
typedef void (*epollHandle)(messageBox*);

typedef void (*commandHandlerPointer)(char*);

int epfd,nfds;

void epollEventDdl(int operation, int fd, int type, epollHandle rFunc,epollHandle wFunc, char *data);
int serverInit();
void epollEventLoop(int listenfd);
void handerAccept(int listenfd);
void readCallback(messageBox *box);
void writeCallback(messageBox *readBox);
void handlerCommand(char *name);
void runCommandHandler(char *name);
void walkCommandHandler(char *name);
void setCommandHandler(char *name);
void getCommandHandler(char *name);

