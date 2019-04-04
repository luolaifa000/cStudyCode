OBJS = main.o client.o pool.o log.o

main : $(OBJS)
	gcc -o main $(OBJS)

main.o : main.c 
	gcc -c main.c ../common/common.h ../common/client.h

client.o : ../common/client.c
	gcc -c ../common/client.c ../common/common.h ../common/client.h

pool.o : ../common/pool.c
	gcc -c ../common/pool.c 

log.o : ../log/log.c
	gcc -c ../log/log.c

clean : 
	rm main $(OBJS)
