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
mqd_t queue[MAX_TASK];

struct data {
    ptypes type;
    union  {
        std::uint8_t a;
        std::uint32_t b;
        char c[32];
    };
};


void *Thread1(void *data) {

//    mgd_t iam = getTaskEntry(1);
    int rc = 0;
    uint8_t msg[255];
    int msgSize = sizeof(msg);
    struct mq_attr attr;
    
    mngmt me ;

    mqd_t iam = mkQueue(1);    
    
    setTaskEntry(1, iam);
    
    rc = mq_getattr(iam,&attr);
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


int main() {
    Small *myDb = new Small();

    bool failFlag = myDb->dbInstall("TEST","ING");  
    failFlag = myDb->dbInstall("FRED","TEST_F");  

    printf("%d\n", failFlag);

    printf("TEST = %s\n", myDb->dbLookup("TEST"));

    nlist *rec;

    rec = myDb->dbLookupRec("TEST");

    myDb->printDb();
}

