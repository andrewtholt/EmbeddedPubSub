#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>     /* pthread functions and data structures */

#include "kvs.h"
#include "proto.h"
#include "pubSub.h"
#include "msg.h"
#include <unistd.h>
/*
struct data {
    enum ptypes type;
    void *value;

    void (*getCB)(void *) ;
    void (*setCB)(void *) ;
    void (*subCB)(void*) ;
    void (*unsubCB)(void *) ;

};
*/

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
        uint32_t tmp = (uint32_t)res->value;
        v = (uint8_t)( tmp & 0xff);
//        v = (uint8_t) res->value;
    }

    return v;
}


const void *get(KVSstore *s, char *name) {
    void *res = kvs_get(s, name);

    return res;
}

void *Thread1(void *data) {

//    mgd_t iam = getTaskEntry(1);
    mqd_t iam = mkQueue(1);

    setTaskEntry(1, iam);

    while(1) {
        printf("Thread 1 going to sleep\n");
        sleep(10);
        printf("Awake.\n");
    }
}

void *Thread2(void *data) {
    mqd_t iam = mkQueue(2);

    setTaskEntry(2, iam);

    while(1) {
        printf("Thread 2 going to sleep\n");
        sleep(11);
        printf("Awake.\n");
    }
}


int main() {

    msgInit();

    mqd_t queueId = getTaskEntry(0);

    pthread_t p_thread1;

    int thr_id = pthread_create(&p_thread1, NULL, Thread1, (void*)NULL);

    Thread2(NULL);

    KVSstore *store = kvs_create(strcmp);
    KVSpair *p; 

    struct data *fred = NULL;
    const struct data *bill = NULL;

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


