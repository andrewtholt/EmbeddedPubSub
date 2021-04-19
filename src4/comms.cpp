#include <cstdio>
#include <limits>
#include <cstdint>
#include <cinttypes>

#include <unistd.h>
#include "comms.h"

mqd_t mkQueue(uint8_t taskId) {

    char buffer[16] = {0};

#ifdef LINUX

    int msgSize = 255;

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

