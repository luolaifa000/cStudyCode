#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "pool.h"

void *defMalloc(int size)
{
    void *temp = malloc(size);
    memset(temp,0,size);
    return temp;
}
