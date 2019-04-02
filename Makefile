demo2 : demo2.o server.o log.o
	gcc -o demo2 demo2.o server.o log.o

demo2.o : demo2.c
	gcc -c demo2.c

server.o : ./common/server.c
	gcc -c ./common/server.c

log.o : ./log/log.c
	gcc -c ./log/log.c

clean : 
	rm demo2
