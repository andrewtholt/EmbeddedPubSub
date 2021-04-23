#include <string.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>

#include "cmdParser.h"
#include "linuxParser.h"
#include "linuxParser.h"
#include "msgs.h"
#include "enums.h"

linuxParser::linuxParser() {
    commonInit();

    data = new Small();

}
linuxParser::linuxParser(Small *db) {
    commonInit();
    data = db;

    data->setCommsCallback(linuxParser::commsCallback);
    // data->setCommsCallback(commsCallback);
}

/*
   void linuxParser::cmdSet(char *m, char *v) {
   bool rc=true;

   rc = data->dbInstall(m,v) ;

   }
   */

/*
 * @param dest Name of message Q to respond to
 * @param cmd Command i.e. SUB|UNSUB|GET|SET|PING|PONG
 * @param key name of variable
 * @param value Value.
 */
bool linuxParser::commsCallback(const mqd_t dest, const enum cmdDef c, const char *key, const char *value) {
    bool failFlag=true;

    struct cmdMessage toGo;
    mqd_t mq = -1;
    int rc=0;

    printf("linuxParser::commsCallback\n");

    memset(&toGo,0,sizeof(struct cmdMessage));

    toGo.payload.message.fields=1;

    if( (dest == NULL) || (c == cmdDef::NOP) ) {
        failFlag = true;
    } else {
        // Assemble the message
        toGo.payload.message.cmd = c; 

        if(cmdDef::SUB == c) {
            toGo.payload.message.fields=2;
            strncpy(toGo.payload.message.key, key, MAX_KEY);
            memset(toGo.payload.message.value, 0, MAX_VALUE);
            failFlag = false;
        }
        if(cmdDef::UNSUB == c) {
            toGo.payload.message.fields=2;
            strncpy(toGo.payload.message.key, key, MAX_KEY);
            memset(toGo.payload.message.value, 0, MAX_VALUE);
            failFlag = false;
        }
        if(cmdDef::GET == c) {
            toGo.payload.message.fields=2;
            strncpy(toGo.payload.message.key, key, MAX_KEY);
            memset(toGo.payload.message.value, 0, MAX_VALUE);
            failFlag = false;
        }
        if(cmdDef::SET == c) {
            toGo.payload.message.fields=3;
            strncpy(toGo.payload.message.key, key, MAX_KEY);
            strncpy(toGo.payload.message.value, value, MAX_VALUE);
            failFlag = false;
        }
        if(cmdDef::PING == c) {
            toGo.payload.message.fields=1;
            toGo.payload.message.cmd = cmdDef::PONG ;
            memset(toGo.payload.message.key, 0, MAX_KEY);
            memset(toGo.payload.message.value, 0, MAX_VALUE);
            failFlag = false;
        }
        //
        // Check failFlag to ensure a valid message.
        //
        if(failFlag == false) {
            // 
            // TODO dest is the mq file descriptor, so no need.
            // to open and close.
            //
            perror("Failed to open destination message Q");
            failFlag = true ;
            if( dest >0) {
                // Q open, so send message
                rc=mq_send(dest, (char *)&toGo , sizeof(struct cmdMessage), 0);
                if( rc < 0 ) {
                    perror("Failed to send message to Q");
                    failFlag = true;
                } else {
                    failFlag = false;
                }
            }
        }
    }
    return failFlag;
}


bool linuxParser::fromMe(struct cmdMessage *m, const mqd_t to) {
    bool rc=true;
    bool opened=false;
    mqd_t mq;
    char *v;

    m->payload.message.fields = 3;

    m->sender == iam;
    m->payload.message.cmd = cmdDef::SET;
    v=data->dbLookup(m->payload.message.key);

    if(v != NULL) {
        strncpy(m->payload.message.value,v,sizeof(m->payload.message.value));
    } else {
        strcpy(m->payload.message.value,"NODATA");
    }

    rc = commsCallback(to, (enum cmdDef)m->payload.message.cmd, m->payload.message.key, m->payload.message.value);
    return rc;
}

char *linuxParser::version() {
    return (char *)"linuxParser";
}

void linuxParser::setIam(mqd_t n) {
    //    strncpy(iam,n,SENDER_SIZE);
    iam = n;
}

mqd_t linuxParser::getIam() {
    return(iam);
}

extern "C" {

    struct linuxParser *newParser(Small *db) {
        return new linuxParser(db);
    }

    void setIam(struct linuxParser *p,mqd_t n) {
        p->setIam(n);
    }

    bool cmdParse(struct linuxParser *p,struct cmdMessage *m,postParseAction_t *act) {
        bool rc=true;

        rc=p->parse(m, act);

        return rc;
    }
}

