/*
#include <stdint.h>
#include <stdbool.h>
*/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "msg.h"

#ifdef LINUX
#include <mqueue.h>
#include <unistd.h>
#endif

#ifdef LINUX
    mqd_t queue[MAX_TASK];
#else
void *queue[ MAX_TASK];
#endif

void setTaskEntry(uint8_t taskId, mqd_t mq) {
    // Acquire lock here
    queue[taskId] = mq;
    // release lock here
}


void *mkQueue(uint8_t taskId) {

    void *ret = NULL;
    char buffer[16] = {0};

#ifdef LINUX

    int msgSize = 255;

    sprintf( buffer, "/dev/mqueue/%02x", taskId);

    printf("Q name is >%s<\n", buffer);

    mqd_t mq;
    struct mq_attr attr;

    attr.mq_flags = 0;    
    attr.mq_maxmsg = 10;    
    attr.mq_msgsize = msgSize;    
    attr.mq_curmsgs = 0;

    int exists = access(buffer, F_OK) ;    
    if( exists !=-1) {    
        printf("Queue %s already exists\n", buffer);    
        mq = mq_open(buffer, O_RDWR | O_NONBLOCK, 0644, &attr);    
    } else {    
        printf("Creating Q\n");    
        mq = mq_open(buffer, O_CREAT | O_RDWR | O_NONBLOCK, 0644, &attr);    
    }    

    if(mq == -1) {    
        perror("mq_open");    
    } else {
        setTaskEntry(taskId, mq);
    }

#endif
    return ret;
}

// 
// Get message int *msg, return length.  -ve indicates eror.
//
int getMsg( uint8_t task, void *msg, uint16_t len ) {
    int ret = -1;
}

int getMsgWait( uint8_t task, void *msg, uint16_t len, uint16_t ms ) {
    int ret = -1;
}
// 
// Put message int *msg, return length.  -ve indicates eror.
//
int putMsg( uint8_t task, void *msg, uint16_t len ) {
    int rc = -1;
}

int putMsgWait( uint8_t task, void *msg, uint16_t len, uint16_t ms ) {
    int rc = -1;
}

int queueLen( uint8_t task) {
    int len = -1;
}

bool queueFull(uint8_t task) {
    bool full = false;
}

bool queueEmpty(uint8_t task) {
    bool empty = false;
}


