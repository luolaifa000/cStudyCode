

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

typedef char* (*commandHandlerPointer)(char*, char*);



int epfd,nfds,epfdChild;

void epollEventDdl(int operation, int fd, int type, epollHandle rFunc,epollHandle wFunc, char *data);

int serverInit();

void epollEventLoop(int listenfd);

void handerAccept(int listenfd);

void readCallback(messageBox *box);

void writeCallback(messageBox *readBox);

char* handlerCommand(char *name);

char* runCommandHandler(char *name, char *value);

char* walkCommandHandler(char *name, char *value);

char* setCommandHandler(char *name, char *value);

char* getCommandHandler(char *name, char *value);

char* filterHuanHang(char *value);

