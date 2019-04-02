main : main.o server.o log.o hashTable.o
	gcc -o main main.o server.o log.o hashTable.o

main.o : main.c 
	gcc -c main.c

server.o : ./common/server.c
	gcc -c ./common/server.c

log.o : ./log/log.c
	gcc -c ./log/log.c

hashTable.o : ./hashTable/hashTable.c
	gcc -c ./hashTable/hashTable.c

clean : 
	rm main
