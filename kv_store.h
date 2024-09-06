#ifndef _KVSTORE
#define _KVSTORE

typedef struct KVP {
    long key;           // the key 
    unsigned int size;  // the size of the value string
    char *val;          // pointer to the value string
    struct KVP *next;   // pointer to the next node
} KVPAIR;

// function declarations
KVPAIR *deserialize(char *buf, size_t buffer_size);
KVPAIR *lookup(KVPAIR *list, long key);
int delete(KVPAIR **list, long key);
void run_tests(int num_pairs, char *test_fname);

void printlist(KVPAIR *list); // this is a useful function for you to use

#endif
