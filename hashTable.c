#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct {
    int index;
    char *key;
    void *next;
    void *pre;
} node;

typedef struct {
    int size;
    void *func;
    node **bucket;
    int countNode;

} hashTable;


typedef int (*hashHandler)(char *key, int size);

int hashFunction(char *key, int size);

int hashTableInsert(hashTable *hash, node *node);

void *defMalloc(int size);

node* createNode(const char *string);

void printfHashTable(hashTable *hash);

int main()
{
    hashTable *hash;
    hash = (hashTable *) defMalloc(sizeof(hashTable));
    hash->size = 16;
    hash->countNode = 0;
    hash->func = hashFunction;
    hash->bucket = (node **) defMalloc(sizeof(node *) * hash->size);
    

    //插入数据
    char arr[][10] = {{"java"},{"php"},{"c"},{"python"},{"go"}};
    int i = 0;
    for (i; i<5 ; ++i)
    {
        node *temp = createNode(arr[i]);
        hashTableInsert(hash, temp);
    }

    //遍历数据打印一下
    printfHashTable(hash);
    
    free(hash->bucket);
    free(hash);
    return 0;
}


void printfHashTable(hashTable *hash)
{
    int k = 0;
    while(k++ < hash->size){
        /* code */
        if (hash->bucket[k])
            printf("index:%d,key:%s\n",hash->bucket[k]->index,hash->bucket[k]->key);
    }
}


int hashTableInsert(hashTable *hash, node *node)
{
    printf("key:%s\n", node->key);
    int index = (*((hashHandler) hash->func))(node->key, hash->size);
    node->index = index;
    printf("index:%d\n", index);
    if (hash->bucket[index]) {
        //地址冲突，找到链表尾节点在后面插入
    } else {
        hash->bucket[index] = node;
    }

    hash->countNode++;
    
    return 0;
}


int hashFunction(char *key, int size)
{
    int temp = 0;
    while(*key) {
        temp += (int) *key;
        key++;
    }
    return temp % size;
}

void *defMalloc(int size)
{
    void *temp = malloc(size);
    memset(temp,0,size);
    return temp;
}


node* createNode(const char *string)
{
    node *temp;
    temp = (node *) defMalloc(sizeof(node));

    temp->key = (char *)string;
    temp->index = 0;
    temp->next = NULL;
    temp->pre = NULL;
    return temp;
}