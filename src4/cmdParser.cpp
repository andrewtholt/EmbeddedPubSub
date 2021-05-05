
#ifdef LINUX
#include <iostream>
#endif

#include <string.h>

#include "Small.h"
#include "cmdParser.h"

#include "enums.h"

// #include "msgs.h"

#ifndef SENDER_SIZE
#define SENDER_SIZE 32
#endif

/**
 * @file cmdParser.cpp
 * @brief Fred
 **/

// using namespace std;

// #ifdef __cplusplus
void parser::commonInit() {

    static bool run=false;

    if(run == false ) {
        run=true;
        dontCreate = false;
        data = NULL;
    }
}

parser::parser() {
    commonInit();
    std::cout << "Creating db\n" ;
    data = new Small();
}

parser::parser( Small *db) {
    commonInit();

    data = db;
}

parser::~parser() {
}

class Small *parser::getDb() {
    return data;
}

// 
// TODO Should this check for db being set and refuse to change it ?
// ... Or should I have a function to clear data thus making this a two step process
//
void parser::setDb(Small *db) {
    data=db;
}

void parser::setDontCreate() {
    dontCreate = true;
}

bool parser::cmdPing(struct cmdMessage *m,postParseAction_t *a) {
    bool failFlag=true;

    m->payload.message.cmd = cmdDef::PONG ;  // Acknowledge PING

    printf("%d\n", (int)m->payload.message.cmd);
    printf("%d\n", (int)cmdDef::PONG);
// TODO: linuxParser
//    failFlag = fromMe(m, (char *)m->sender);
    failFlag = fromMe(m, m->sender);
    failFlag = false;

    return failFlag;
}

bool parser::cmdSub(struct cmdMessage *m,postParseAction_t *a) {
    bool failFlag=true;
    enum psError rc=PS_OK;

    class nlist *rec=NULL;

    rec = data->dbLookupRec(m->payload.message.key);
    if(!rec) {
        failFlag=true;
    } else {        
        rc=data->subscribe((void *)m->sender, m->payload.message.key);

        if ( rc == PS_OK) {
            // TODO See linuxParser
//        	failFlag=fromMe(m, (char *)m->sender);
        }
    }
    return failFlag;
}

bool parser::cmdUnsub( cmdMessage *m,postParseAction_t *a ) {
	bool failFlag=true;
	//
	// This can't fail.  An attempt to unsubscribe from either a non existant variable,
	// or from one we are not subscribed to is harmless.
	// Catch the reason for debugging.
	//
	enum psError res = data->unsubscribe((void *) m->sender,m->payload.message.key);

	if(res != PS_OK) {
		failFlag = true;
	} else {
		failFlag = false;
	}
	*a=PP_FREE_MEM;
	return failFlag ;
}

bool parser::cmdGet(struct cmdMessage *m, postParseAction_t *a) {
    bool rc=true;

    char *v;

#ifdef LINUX
//    char destination[SENDER_SIZE];
    uint8_t destination = 0;
#else
    QueueHandle_t destination;
#endif

    // TODO Change this to dbLookupRec
    v=data->dbLookup( m->payload.message.key);
    if( v == NULL ) {
        strncpy(m->payload.message.value,(char *)"NOVAR",MAX_VALUE);
    } else {
        // TODO Check here for confidential flag.
        // If set return 'NOVAR'

        strncpy(m->payload.message.value,v,MAX_VALUE);
    } 
#ifdef LINUX
//    memcpy(destination, m->sender,SENDER_SIZE);
    destination=m->sender;
#else
    destination=m->sender;
#endif

    // TODO See linuxParser
//    rc=fromMe(m,destination);

    *a=PP_NULL; // No further action.

    return rc;
}
// Platform specific.
// Assumes file access will be done via a thread/task dedicated to that role.
//
bool parser::cmdOpen(struct cmdMessage *m, postParseAction_t *a) {
	bool failFlag=true;

	return failFlag;
}
/*
 * @name cmdSet
 */
bool parser::cmdSet(struct cmdMessage *m, postParseAction_t *a) {
    bool failFlag=true;

//    *a=PP_NULL;

    if(dontCreate) {
        if( !data->dbLookup(m->payload.message.key)) {
            // Not found and don't create.
            failFlag=true;
        } else {
            // TODO: If confidential set acti as if not found.
            //
            // otherwise found, so update it.
            //
            // TODO If read only is set then do nothing.
            failFlag=data->dbInstall(m->payload.message.key,m->payload.message.value);
        }
    } else {
        // can create so get on with it

        failFlag=data->dbInstall(m->payload.message.key, m->payload.message.value);
    }
//    *a=PP_FREE_MEM;
//    *a=PP_NULL;
    return failFlag;
}

void parser::msgDump(struct cmdMessage *ptr) {
    printf("Sent by     : %d\n",ptr->sender);
    printf("Field Count : %d\n",ptr->payload.message.fields);
    printf("Command     : %d\n",ptr->payload.message.cmd);

    if(ptr->payload.message.fields > 1 ) {
        printf("Key         : %s\n",ptr->payload.message.key);
    } else {
        printf("Key         : INVALID\n");
    }


    if( ptr->payload.message.fields > 2 ) {
        printf("Value       : %s\n",ptr->payload.message.value);
    } else {
        printf("Value       : INVALID\n");
    }
}

bool parser::parse(struct cmdMessage *m, postParseAction_t *act){
    bool failFlag=true;

    if(data == NULL) {
        data = newSmall();
    }


    switch ( m->payload.message.fields) {
        case 1:
            if(m->payload.message.cmd == (uint8_t)cmdDef::PING ) {
                failFlag = cmdPing(m, act);
            } else if(m->payload.message.cmd == (uint8_t)cmdDef::PONG ) {
                failFlag=false;
            } else {
                failFlag = true;
            }
            break;
        case 2: // GET, SUB
            if( m->payload.message.cmd == cmdDef::GET ) {
                failFlag = cmdGet(m, act);
            } else if( m->payload.message.cmd == cmdDef::UNSUB ) {
                failFlag = cmdUnsub(m, act);
            } else if( m->payload.message.cmd == cmdDef::SUB ) {
                failFlag = cmdSub(m,act);
            } else {
                failFlag = true;
            }
            break;
        case 3: // SET
            if( m->payload.message.cmd == cmdDef::SET ) {
                failFlag = cmdSet(m, act);
            } else {
                failFlag=true;
            }
            break;
        default:
            failFlag=true;
            break;
    }

    return failFlag;
}


// #endif

extern "C" class Small *parserGetDb(struct parser *p) {
    return ( p->getDb() );
}

extern "C" bool parserParse(struct parser *p, struct cmdMessage *m) {

	bool failFlag=true;
	postParseAction_t act;

    failFlag=p->parse(m, &act);
    return (failFlag);
}

extern "C" void parserSetDontCreate(struct parser *p) {
    p->setDontCreate();
}


