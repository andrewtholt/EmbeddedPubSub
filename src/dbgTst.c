#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "kvs.h"
#include "proto.h"

struct data {
    enum ptypes type;
    void *value;

    void (*getCB)(void *) ;
    void (*setCB)(void *) ;
    void (*subCB)(void*) ;
    void (*unsubCB)(void *) ;

};

void setDefaultCB(struct data *res) {
    res->getCB = NULL;
    res->setCB = NULL;
    res->subCB = NULL;
    res->unsubCB = NULL;
}

void set(KVSstore *s, char *name, struct data *v) {
    kvs_put(s, name , v);
}

void setBoolean(KVSstore *s, char *name, bool state) {

    // db lock here
    struct data *res = kvs_get(s, name);

    if (res == NULL) {
        res = calloc(1,sizeof(struct data));
        setDefaultCB(res);
        res->type=BOOL;
    }
// 
// TODO: Check if record is a BOOL
// If not ??
//
    res->value = (state == true)? (void *) 1 : NULL;

    kvs_put(s, name , res);
    // db unlock here
}
// 
// Lookup value and return boolean state
//
// TODO: Check if record is a BOOL
// If not ??
//
bool getBoolean(KVSstore *s, char *name) {
    bool v=false;

    struct data *res = kvs_get(s, name);
    if(res == NULL) {
        v=false;
    } else {
        v = (res == NULL) ? false : true ;
    }

    return v;
}

void setByte(KVSstore *s, char *name, uint8_t state) {
    // db lock here
    struct data *res = kvs_get(s, name);

    if (res == NULL) {
        res = calloc(1,sizeof(struct data));
        setDefaultCB(res);
        res->type=BYTE;
    }
// 
// TODO: Check if record is a BYTE
// If not ??
//
    res->value = (void *)state;

    kvs_put(s, name , res);
    // db unlock here

}
// 
// Lookup value and return byte
//
// TODO: Check if record is a BYTE
// If not ??
//
uint8_t getByte(KVSstore *s, char *name) {
    uint8_t v=0;

    struct data *res = kvs_get(s, name);
    if(res == NULL) {
        v=0;
    } else {
        v = (uint8_t) res->value;
    }

    return v;
}


void *get(KVSstore *s, char *name) {
    void *res = kvs_get(s, name);

    return res;
}

int main() {
    KVSstore *store = kvs_create(strcmp);
    KVSpair *p; 

    struct data *fred = calloc(1,sizeof(struct data));
    struct data *bill = NULL;

    fred->type = BOOL;

    set(store,"FRED", fred);

    bill = get(store,"FRED");

    setBoolean(store,"BILL", true);

    printf("Here\n");

    bool t = getBoolean(store,"BILL");

    printf("getByte 0x%02x\n", 42);
    setByte(store, "JOE" , 42);

    uint8_t r = getByte(store, "JOE");

    printf("getByte 0x%02x\n", r);
}


