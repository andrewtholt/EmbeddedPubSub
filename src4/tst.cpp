#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <pthread.h>	/* pthread functions and data structures */
#include <strings.h>
#include <time.h>
#include "enums.h"
#include "Small.h"
#include "msgs.h"
#include "comms.h"
#include "mngmt.h"

#include "cmdParser.h"
#include "linuxParser.h"

using namespace std;
// mqd_t queue[MAX_TASK];
//

mngmt *top = mngmt::Instance();

struct data {
    ptypes type;
    union  {
        std::uint8_t a;
        std::uint32_t b;
        char c[32];
    };
};

void addTime(struct timespec *tm, int msec) {
    clock_gettime(CLOCK_MONOTONIC, tm);

    tm->tv_nsec += msec * 1000000;
    if( tm->tv_nsec >= 1000000000 ) { 
          tm->tv_nsec -= 1000000000;
            tm->tv_sec++;
    }   
}

void cmdToString(enum cmdDef c) {

    switch(c) {
        case(cmdDef::NOP):
            printf("NOP");
            break;
        case(cmdDef::GET):
            printf("GET");
            break;
        case(cmdDef::SET):
            printf("SET");
            break;
        case(cmdDef::SUB):
            printf("SUB");
            break;
        case(cmdDef::UNSUB):
            printf("UNSUB");
            break;
        case(cmdDef::PING):
            printf("PING");
            break;
        case(cmdDef::PONG):
            printf("PONG");
            break;
        case(cmdDef::EXIT):
            printf("EXIT");
            break;
        default:
            printf("UNKNOWN");
            break;
    }

}

void *Thread1(void *data) {

    int rc = 0;
    struct timespec tm;

    int msgSize = sizeof(struct cmdMessage);
    struct cmdMessage *msg = (struct cmdMessage *)malloc(msgSize);
   
    linuxParser *msgParser = new linuxParser();

    bool initFailed = top->initTask(tasks::TASK1);
    mqd_t iam = top->getTaskEntry(tasks::TASK1);

    msgParser->setIam(iam);
    
    while(1) {
        printf("Thread 1 waiting for msg\n");

        bzero(msg, msgSize);
        rc = mq_receive(iam, (char *)msg, msgSize, 0);
        if(rc < 0) {
            perror("Thread1:");
            exit(2);
        } else {
            bool failFlag = msgParser->parse(msg, NULL);
            if(failFlag) {
                printf("parse failed\n");
            } else {
                printf("parse success\n");
                printf("T1RX CMD->");
                cmdToString(msg->payload.message.cmd);
                printf("\n");
//                msgParser->msgDump(msg);
            }

        }
    }
}

void *Thread2(void *data) {    

//    struct cmdMessage outMsg;
    struct cmdMessage outMsg;
    struct cmdMessage inMsg;
    int rc=-1;

    uint8_t msgSize = sizeof(outMsg);
    struct timespec tm;

    linuxParser *msgParser = new linuxParser();

    bool initFailed = top->initTask(tasks::TASK2);
    mqd_t iam = top->getTaskEntry(tasks::TASK2);
    
    
    if( !top->waitUntilReady(tasks::TASK1) ) {    
        fprintf(stderr, "T2:waitUntilReady fail.\n");    
        exit(1);    
    }

    mqd_t dest = top->getTaskEntry(tasks::TASK1);

    int counter = 1;
    while(1) {
        addTime(&tm, 400);

        bzero(&outMsg,msgSize);
        bzero(&inMsg,msgSize);

        outMsg.sender = iam;
        outMsg.payload.message.fields = 1;
        outMsg.payload.message.cmd = cmdDef::PING;

        rc = mq_timedreceive(iam, (char *)&inMsg, msgSize, 0,&tm);
        if(rc < 0) {
            printf("T2:Waiting over %d\n",rc);
        } else {
            bool failFlag = msgParser->parse(&inMsg, NULL);
            msgParser->msgDump(&inMsg);
            printf("T2:RX CMD->");
            cmdToString(outMsg.payload.message.cmd);
            printf("\n");
        }

        printf("TX CMD->");
        cmdToString(outMsg.payload.message.cmd);
        printf("\n");

        int rc = mq_send(dest, (char *)&outMsg, msgSize, 0);

        counter++;
        sleep(4);
        printf("Awake.\n");
    }
}


int main() {

    pthread_t t1Id;
    pthread_t t2Id;

    int thread2Status = pthread_create(&t1Id, NULL, *Thread2, NULL);

    int thread1Status = pthread_create(&t1Id, NULL, *Thread1, NULL);

    sleep(100);

    /*
    Small *myDb = new Small();

    bool failFlag = myDb->dbInstall("TEST","ING");  
    failFlag = myDb->dbInstall("FRED","TEST_F");  

    printf("%d\n", failFlag);

    printf("TEST = %s\n", myDb->dbLookup("TEST"));

    nlist *rec;

    rec = myDb->dbLookupRec("TEST");

    myDb->printDb();
    */
}

