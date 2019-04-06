#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <error.h>
#include <fcntl.h>
#include <sys/stat.h>

int deamonInit()
{
    int fd;
    switch (fork()) {
    case -1:
        printf("fork() error\n");
        exit(1);
        return 1;

    case 0:
        break;

    default:
        exit(0);
    }

    setsid();
    umask(0);

    fd = open("/dev/null", O_RDWR);
    if (fd == -1) {
        printf("open() /dev/null error\n");
        return 1;
    }

    if (dup2(fd, STDIN_FILENO) == -1) {
        printf("dup2(STDIN) failed");
        return 1;
    }

    if (dup2(fd, STDOUT_FILENO) == -1) {
        printf("dup2(STDOUT) failed");
        
    }

    if (fd > STDERR_FILENO) {
        if (close(fd) == -1) {
            printf("close() failed\n");
            return 1;
        }
    }

    return 0;
}