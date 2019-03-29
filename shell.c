#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <error.h>

#include "deamon/deamon.h"



int main()
{
    deamonInit();
    FILE *fp;
    //char *string;
    //string = (char *) malloc(1000);
    //fp = popen("", "r");
    //fgets(string, 1000, fp);
    //printf("%s\n",string);
    //pclose(fp);
    while (1) {
        sleep(1);
        fp = popen("echo | ps -ef|grep nginx >> /mnt/windows/cStudyCode/demo.log", "r");
    }
    pclose(fp);
}


