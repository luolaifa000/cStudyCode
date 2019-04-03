# cStudyCode
cStudyCode

```gcc
##epoll

gcc main.c ./common/server.c -o main

gcc main.c ./common/server.c ./log/log.c -o main

##守护进程
gcc shell.c ./deamon/deamon.c -o shell

##hashtable
gcc hashTable.c -o hash
```


```tip
如果自定义模块功能的话
1.例如这个log，如果没有自定义的全局变量或者常量的话不需要定义头文件
2.例如这个hashtable，如果有自定义的全局变量或者常量的话需要定义头文件,其他模块需要调用的时候需要包含头文件
```


#makefile
`
使用makefile编译工程
`