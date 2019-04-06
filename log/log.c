#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>


void commandLog(const char *filename, char *string)
{
    int fd;
    fd = open(filename,O_RDWR|O_CREAT|O_APPEND, "0777");
    write(fd, string, strlen(string));
    close(fd);
}


void mcLog(const char *filename, char *string)
{
    int fd;
    char *s = (char *) calloc(256, sizeof(char));
    char *start = s;
    time_t curtime;
    time(&curtime);
    char *date = ctime(&curtime);

    date[strlen(date) - 1] = '\0';
    while(*date && strlen(start) < 256) {
        *s++ = *date++;
    }
    *s++ = ' ';

    char *info = "[info]";

    while(*info && strlen(start) < 256) {
        *s++ = *info++;
    }

    *s++ = ' ';
    while(*string && strlen(start) < 256) {
        *s++ = *string++;
    }
    *s++ = '\n';

    umask(0000);

    fd = open(filename,O_RDWR|O_CREAT|O_APPEND, "0777");

    if (fd == -1) {
        fprintf(stderr, "open() Error opening file: %s,filename:%s,string:%s\n", strerror(errno),filename, start);
    }


    int count = write(fd, start, strlen(start));

    if (count == -1) {
        fprintf(stderr, "write() Error opening file: %s,filename:%s,string:%s\n", strerror(errno),filename, start);
    }
    close(fd);
    free(start);
}

