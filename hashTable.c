#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct {
    int index;
    char *key;
    void *next;
    void *pre;
} hashNode;

typedef struct {
    int size;
    void *func;
    hashNode **bucket;
    int countNode;

} hashTable;


typedef int (*hashHandler)(char *key, int size);

int hashFunction(char *key, int size);

int hashTableInsert(hashTable *hash, hashNode *node);

void *defMalloc(int size);

hashNode* createNode(const char *string);

void printfHashTable(hashTable *hash);

int main()
{
    hashTable *hash;
    hash = (hashTable *) defMalloc(sizeof(hashTable));
    hash->size = 16;
    hash->countNode = 0;
    hash->func = hashFunction;
    hash->bucket = (hashNode **) defMalloc(sizeof(hashNode *) * hash->size);
    

    //插入数据
    char arr[][10] = {{"java"},{"php"},{"c"},{"python"},{"go"}};
    int i = 0;
    for (i; i<5 ; ++i)
    {
        hashNode *temp = createNode(arr[i]);
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
    hashNode *p;
    printf("begin print hashTable:%d\n",hash->countNode);
    while(k++ < hash->size){
        /* code */
        if (hash->bucket[k]) {
            printf("index:%d,key:%s\n",hash->bucket[k]->index,hash->bucket[k]->key);
            p = hash->bucket[k];
            while(p->next) {
                printf("index:%d,key:%s\n",((hashNode *)(p->next))->index,((hashNode *)(p->next))->key);
                p = (hashNode *) p->next;
            }
        }
            
    }
}


int hashTableInsert(hashTable *hash, hashNode *node)
{
    printf("key:%s\n", node->key);
    int index = (*((hashHandler) hash->func))(node->key, hash->size);
    node->index = index;
    printf("index:%d\n", index);
    hashNode *p = hash->bucket[index];
    if (p) {
        //地址冲突，找到链表尾节点在后面插入
        while(p) {
            if (strcmp(node->key, p->key) == 0) {
                return 1;
            }
            if (strcmp(node->key, p->key) < 0) {
                node->next = p;

                
            }
            p = (hashNode *) p->next;
        }
        p = node;

    } else {
        p = node;
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


hashNode* createNode(const char *string)
{
    hashNode *temp;
    temp = (hashNode *) defMalloc(sizeof(hashNode));

    temp->key = (char *)string;
    temp->index = 0;
    temp->next = NULL;
    temp->pre = NULL;
    return temp;
}