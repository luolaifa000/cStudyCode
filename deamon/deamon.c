#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <error.h>

void deamonInit()
{
    int pid = fork();

    if (pid < 0) {
        printf("fork\n");
        exit(1);
    }

    if (pid > 0) {
        exit(0);
    }

    setsid();

    int pid2 = fork();

    if (pid2 < 0) {
        printf("fork\n");
        exit(1);
    }

    if (pid2 > 0) {
        exit(0);
    }
   
    umask(0);
}