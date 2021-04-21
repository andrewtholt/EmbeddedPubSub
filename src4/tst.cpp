#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <pthread.h>	/* pthread functions and data structures */
#include <strings.h>

#include "enums.h"
#include "Small.h"
#include "msgs.h"
#include "comms.h"
#include "mngmt.h"

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


void *Thread1(void *data) {

    int rc = 0;
    /*
    uint8_t msg[255];
    int msgSize = sizeof(msg);
    */

    int msgSize = sizeof(struct cmdMessage);
    struct cmdMessage *msg = (struct cmdMessage *)malloc(msgSize);
    
    bool initFailed = top->initTask(1);

    mqd_t iam = top->getTaskEntry(1);
    
    while(1) {
        printf("Thread 1 waiting for msg\n");

        bzero(msg, msgSize);
        rc = mq_receive(iam, (char *)msg, msgSize, 0);
        if(rc < 0) {
            perror("Thread1:");
            exit(2);
        } else {
            printf("rx >%s<\n",msg);
        }
    }
}

void *Thread2(void *data) {    
    char buffer[32];    
    
    bool initFailed = top->initTask(1);
    
    if( !top->waitUntilReady(1) ) {    
        fprintf(stderr, "T2:waitUntilReady fail.\n");    
        exit(1);    
    }

    mqd_t dest = top->getTaskEntry(1);

    int counter = 1;
    while(1) {
        bzero(buffer,32);
        sprintf(buffer,"%04d",counter);

        printf("tx %04d\n", counter);
        int rc = mq_send(dest, buffer, 32, 0);

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

    Small *myDb = new Small();

    bool failFlag = myDb->dbInstall("TEST","ING");  
    failFlag = myDb->dbInstall("FRED","TEST_F");  

    printf("%d\n", failFlag);

    printf("TEST = %s\n", myDb->dbLookup("TEST"));

    nlist *rec;

    rec = myDb->dbLookupRec("TEST");

    myDb->printDb();
}

