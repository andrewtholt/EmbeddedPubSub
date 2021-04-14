#include <stdio.h>
#include "kvs.h"

struct data {
    int a;
    int b;
};

int main() {
    KVSstore *store = kvs_create(strcmp);
    KVSpair *p;

    int t = 0x30;

    struct data fred;

    fred.a=4;
    fred.b=2;

    kvs_put(store, "abc", "123");
    kvs_put(store, "ghi", "789");
    kvs_put(store, "def", "456");
    kvs_put(store, "jkl", &fred);

    kvs_put(store, "xyz", &t);

    void *x = (int *)&t;
    x = (int *)kvs_get(store,"jkl");

    printf("x=0x%02x\n", *(int*)x);
    printf("t=0x%02x\n", t);


    struct data *d = (struct data *)kvs_get(store,"jkl");
//    char *d = (char *)kvs_get(store,"ghi");

//    printf("Found %s\n",d);

    printf("Members = %d\n", (int)kvs_length(store));

    int i = 0;
    while ((p = kvs_pair(store, i++))) {
        printf("%s: %s\n", (char *)p->key, (char *)p->value);
    }
}
