OBJS = main.o server.o hashTable.o log.o

main : $(OBJS)
	gcc -o main $(OBJS)

main.o : main.c 
	gcc -c main.c ../hashTable/hashTable.h ../common/common.h ../common/server.h

server.o : ../common/server.c
	gcc -c ../common/server.c ../common/common.h ../common/server.h


hashTable.o : ../hashTable/hashTable.c
	gcc -c ../hashTable/hashTable.c 


log.o : ../log/log.c
	gcc -c ../log/log.c

clean : 
	rm main $(OBJS)
