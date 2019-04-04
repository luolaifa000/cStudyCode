OBJS = main.o client.o hashTable.o log.o

main : $(OBJS)
	gcc -o main $(OBJS)

main.o : main.c 
	gcc -c main.c ../common/common.h ../common/client.h

client.o : ../common/client.c
	gcc -c ../common/client.c ../hashTable/hashTable.h ../common/common.h ../common/client.h

hashTable.o : ../hashTable/hashTable.c
	gcc -c ../hashTable/hashTable.c 

log.o : ../log/log.c
	gcc -c ../log/log.c

clean : 
	rm main $(OBJS)
