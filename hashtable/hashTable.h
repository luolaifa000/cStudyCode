#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int index;
    char *key;
    void *next;
    void *pre;
    char *value;
} hashNode;

typedef struct {
    int size;
    void *func;
    hashNode **bucket;
    int countNode;

} hashTable;



hashTable* hashInit(int size);

typedef int (*hashHandler)(char *key, int size);

int hashFunction(char *key, int size);

int hashTableInsert(hashTable *hash, hashNode *node);

char* hashTableFind(hashTable *hash, char *key);

int hashTableDelete(hashTable *hash, char *key);

hashNode* createNode(const char *string, const char *value);

void printfHashTable(hashTable *hash);