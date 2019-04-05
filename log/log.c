#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
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

    fd = open(filename,O_RDWR|O_CREAT|O_APPEND, "0777");
    write(fd, start, strlen(start));
    close(fd);
    free(start);
}

