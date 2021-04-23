#include <stddef.h>  // defines NULL
#include <stdio.h>
#include "mngmt.h"

#include "msgs.h"
#include <mqueue.h>

// Global static pointer used to ensure a single instance of the class.
mngmt* mngmt::m_pInstance = NULL;  

/** This function is called to create an instance of the class. 
  Calling the constructor publicly is not allowed. The constructor 
  is private and is only called by this Instance function.
  */

mngmt::mngmt() {
    printf("Constructor\n");
    pthread_mutex_init(&queueLock, NULL);
    memset(queue,0,sizeof(queue));
}

mngmt* mngmt::Instance() {
    printf("Instance\n");

    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new mngmt;
    } else {
        printf("Exists\n");
    }

    return m_pInstance;
}

bool mngmt::initTask(uint8_t taskId) {
    bool fail = true ;

    // If this taskId is ready, then it's already running
    //
    if( isTaskReady(taskId) ) {
        fail = true;
    } else {
        mkQueue(taskId);
        fail = false;
    }

    return fail ;
}

void mngmt::dump() {

    for (int i=0; i< MAX_TASK; i++) {
        printf("%2d:%08x\n", i, queue[i]);
    }

}

void mngmt::setTaskEntry(uint8_t taskId, mqd_t mq) {
    // Acquire lock here
    pthread_mutex_lock(&queueLock);
    queue[taskId] = mq;
    // release lock here
    pthread_mutex_unlock(&queueLock);
}

mqd_t mngmt::getTaskEntry(uint8_t taskId) {
    mqd_t pipe=0;

    // Acquire lock here
    pthread_mutex_lock(&queueLock);
    pipe = queue[taskId] ;
    // release lock here
    pthread_mutex_unlock(&queueLock);

    return pipe;
}


mqd_t mngmt::mkQueue(uint8_t taskId) {

    char buffer[16] = {0};

#ifdef LINUX

    int msgSize = sizeof(struct cmdMessage);

    //    sprintf( buffer, "/dev/mqueue/%02x", taskId);
    sprintf( buffer, "/%02x", taskId);

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
        mq = mq_open(buffer, O_RDWR, 0644, &attr);
    } else {
        printf("Creating Q\n");
        mq = mq_open(buffer, O_CREAT | O_RDWR, 0644, &attr);
    }

    if(mq == -1) {
        perror("mq_open");
    } else {
        setTaskEntry(taskId, mq);
    }

#endif
    return mq;
}

bool mngmt::isTaskReady(uint8_t taskId) {
    bool ready=false;

    // lock here
    pthread_mutex_lock(&queueLock);
    ready = (queue[taskId] == 0) ? false : true ;
    pthread_mutex_unlock(&queueLock);
    // unlock here

    return ready;
}

bool mngmt::waitUntilReady(uint8_t taskId, uint8_t ms) {
    bool ready = false;

    mqd_t q = -1; 

    while(!ready) {
        q = getTaskEntry(taskId);

        if ( q == 0) {
            usleep(ms * 1000);
        } else {
            ready = true;
            break;
        }
    }   
    return ready ;
}

bool mngmt::waitUntilReady(uint8_t taskId) {
    bool ready = waitUntilReady(taskId,1);

    return ready;
}
