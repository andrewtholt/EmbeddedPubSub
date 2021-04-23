// #include "msgs.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Small.h"
#include "enums.h"
#ifdef LINUX
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
#include <iostream>
#include <unistd.h>
#endif
#ifdef FREERTOS
#include "cmsis_os.h"
#include "yaffsfs.h"
#endif

#ifndef SENDER_SIZE
#define SENDER_SIZE 32
#endif

// using namespace std;

#ifdef LINUX
#include <pthread.h>
#include <mqueue.h>
#endif
// $Id$
/**
 *
 * @file Small.cpp
 * @brief Publish Subscribe database.
 */
Small::Small() {

#ifdef LINUX
    printf("Small Created\n");

    if(pthread_mutex_init(&dbLock,NULL) !=0 ) {
        perror("pthread_mutex_init");
        exit(1);
    }
#endif

#ifdef FREERTOS
//    SemaphoreHandle_t dbLock = xSemaphoreCreateMutex();
     dbLock = xSemaphoreCreateMutex();
#endif

    // Set callback to use defaults

    subCount = 0;
    // globalCallback=NULL;
    
    // TODO: Set default callback
    commsCallback=NULL;

//    freeList = (nlist **)malloc((unsigned int)(sizeof(nlist) * MAX_REC));

    for(int i=0;i<MAX_REC;i++) {
        freeList[i] = NULL;
//        freeList[i] = new nlist();
    }

    hash = new hashtab();
}

#ifdef FREERTOS
#include "usb_printf.h"
void printBoolean(bool v) {
	if(v) {
		usb_printf((char *)"True\n");
	} else {
		usb_printf((char *)"False\n");
	}
}
#endif

#ifdef LINUX
void printBoolean(bool v) {
	if(v) {
		printf((char *)"True\n");
	} else {
		printf((char *)"False\n");
	}
}

#endif

// Output the configuration out to a .ini file.
// TODO: Tidy this
//
void Small::dumpIni(char *path) {
	class nlist *ptr=NULL;
	class nlist *var=NULL;
	class nlist *section=NULL;
	char *name=NULL;
	char tmpBuffer[MAX_NAME];

	char opBuffer[132];
	int fd;

	// TODO if the file exists rename it to path.bak (overwriting any existant file with that name)
	// Then create a new file wit 'path' as its name.
	//
#ifdef LINUX
	if( access(path, 0) < 0) {
		fd = open(path, O_WRONLY|O_CREAT, 0666);
	} else {
		fd = open(path, O_WRONLY|O_TRUNC, 0666);
	}
#endif

#ifdef FREERTOS

	if( yaffs_access(path, 0) < 0) {
		fd = yaffs_open(path, O_WRONLY|O_CREAT, 0666);
	} else {
		fd = yaffs_open(path, O_WRONLY|O_TRUNC, 0666);
	}
#endif

	if( fd < 0) {
		return;
	}

	for(int i=0;i<HASHSIZE;i++) {
		ptr=hash->getHead(i);
		if( ptr != NULL) {
			// is this a section header ?
			do {
				if(ptr->getSection()) {
					section = ptr;
					var=ptr->getSectionNext();
					if(var != NULL) {
//						usb_printf((char *)"[%s]\n", ptr->getName());
						sprintf(opBuffer, (char *)"[%s]\n", ptr->getName());
#ifdef FREERTOS
						yaffs_write(fd, opBuffer, strlen(opBuffer));
#endif

#ifdef LINUX
						write(fd, opBuffer, strlen(opBuffer));
#endif

						ptr=ptr->getSectionNext();
						do {
							strncpy(tmpBuffer,ptr->getName(), MAX_NAME);
							strtok(tmpBuffer,".");
							name=strtok(NULL,".");

							sprintf(opBuffer, "%s=%s\n", name,ptr->getDef());
//							usb_printf("%s=", name);
//							usb_printf("%s\n", ptr->getDef());
#ifdef FREERTOS
							yaffs_write(fd, opBuffer, strlen(opBuffer));
#endif

#ifdef LINUX
							write(fd, opBuffer, strlen(opBuffer));
#endif

							ptr=ptr->getSectionNext();

						}while(ptr != NULL);
					}
				}
				if(section != NULL) {
					ptr=section->getNext();
				} else {
					ptr=ptr->getNext();
				}
			} while(ptr != NULL);
		}
	}
#ifdef FREERTOS
	yaffs_close(fd);
#endif

#ifdef LINUX
	close(fd);
#endif
}

void Small::printDb() {
    class nlist *ptr;

#ifdef LINUX
    pthread_mutex_lock(&dbLock);
    printf((char *)"Sub Count: %d\n", getSubCount());
#endif

//    std::cout << "Sub Count: " + to_string( getSubCount() ) + "\n");
    printf((char *)"Sub Count: %d\n", getSubCount());
    for(int i=0;i<HASHSIZE;i++) {
        ptr=hash->getHead(i);

        if( ptr == NULL) {
            printf((char *)"%4d: Empty\n",i);
        } else {
            do {
            	printf((char *)"\nidx=%4d\n",i);
                printf((char *)"Name         : %s\n", ptr->getName());
                printf((char *)"Section      : ");

                printBoolean(ptr->getSection());

                printf((char *)"Value        : %s\n", ptr->getDef());
                printf((char *)"Sub-Count    : %d\n", ptr->getSubCount());

                printf((char *)"Record       :%08x\n", ptr);
                printf((char *)"Section Link :%08x\n", ptr->getSectionNext());

                printf((char *)"Published    : ");
                printBoolean(ptr->amIPublished());

                printf((char *)"Private      : ");
                printBoolean(ptr->getConfidential());

                printf((char *)"Read Only    : ");
                printBoolean(ptr->getReadOnly());

                ptr=ptr->getNext();
            } while( ptr != NULL);
        }
    }
#ifdef LINUX
    pthread_mutex_unlock(&dbLock);
#endif
}

nlist *Small::dbLookupRec( const char *n) {
    uint32_t h = 0;
    class nlist *ptr;

    h = calcHashval( n );

#ifdef LINUX
    pthread_mutex_lock(&dbLock);
#endif

#ifdef FREERTOS
//    xSemaphoreTake(dbLock,portMAX_DELAY);
    osSemaphoreWait(dbLock, osWaitForever);
#endif

    ptr=hash->getHead( h );
    if(ptr) {
        do {
            if(!strcmp(ptr->getName(),n)) {
                break;
            } else {
                ptr=ptr->getNext();
            }

        } while(ptr != NULL);
    }
#ifdef LINUX
    pthread_mutex_unlock(&dbLock);
#endif
#ifdef FREERTOS
//    xSemaphoreGive(dbLock);
    osSemaphoreRelease(dbLock);
#endif
    return ptr;
}
// If name not found return false
//
bool Small::dbLookupBool(char *name) {
	bool rc=false;

	class nlist *ptr = dbLookupRec(name);

	if( ptr != NULL) {
		rc=ptr->getDefBool();
	}

	return rc;
}
//  If name not found, return default.
//
bool Small::dbLookupBool(char *name, bool dflt) {
	bool rc=dflt;

	class nlist *ptr = dbLookupRec(name);

	if( ptr != NULL) {
		rc=ptr->getDefBool();
	}

	return rc;
}

bool Small::publish(char *name ) {

    bool rc=true;
    class nlist *rec;

    rec=dbLookupRec( name );

    if( rec != NULL) {
        rec->publish();
        /*
        if( globalCallback != NULL ) {
            //            rec->cmdSubscribeCallback( globalCallback);
        }
        */
        rc=false;
    } else {
        rc=true;
    }

    return rc;
}

int32_t Small::getSubCount() {
    return subCount;
}

void Small::incSubCount() {
    subCount++;
}

void Small::decSubCount() {
    subCount--;
}


bool Small::readOnly(char *name, bool ro) {
    bool failFlag=true;
    class nlist *rec;

    rec=dbLookupRec( name );
    if( rec != NULL) {
        rec->setReadOnly( ro );
        failFlag=false;
    } else {
        failFlag=true;
    }
    return failFlag;
}

bool Small::confidential(char *name, bool c ) {
    bool failFlag=true;
    class nlist *rec;

    rec=dbLookupRec( name );
    if( rec != NULL) {
        rec->setConfidential( c );
        failFlag=false;
    } else {
        failFlag=true;
    }

    return failFlag;
}


char *Small::dbLookup( const char *n) {
    uint32_t h = 0;
    class nlist *ptr;
    char *res=NULL;

    h = calcHashval( n );
#ifdef LINUX
    pthread_mutex_lock(&dbLock);
#endif

#ifdef FREERTOS
//    xSemaphoreTake(dbLock, portMAX_DELAY );
    osSemaphoreWait(dbLock, osWaitForever);
#endif

    ptr=hash->getHead( h );
    if(!ptr) {
        res=NULL;
    } else {
        do {
            if(!strcmp(ptr->getName(),n)) {
                res=ptr->getDef();
                break;
            } else {
                ptr=ptr->getNext();
            }

        } while(ptr != NULL);
    }
#ifdef LINUX
    pthread_mutex_unlock(&dbLock);
#endif

#ifdef FREERTOS
//    xSemaphoreGive(dbLock);
    osSemaphoreRelease(dbLock);
#endif
    return res;
}


#ifdef LINUX
bool Small::setCommsCallback(bool (*cb)(const mqd_t , const enum cmdDef cmd, const char *key, const char *value)) {
#else
bool Small::setCommsCallback(bool (*cb)(const QueueHandle_t dest, const char *cmd, const char *key, const char *value)) {
#endif
    bool failFlag=true;

    commsCallback=cb;
    failFlag=false;

    return failFlag;
}

bool Small::setSetCallback(char *k, void (*cb)(class nlist *)) {
    bool failFlag=true;

    struct nlist *rec;

    rec=dbLookupRec( k ) ;
    if(rec == NULL) {
        failFlag=true;
    } else {
        rec->cmdSetCallback( cb );
    }

    return failFlag;
}

bool Small::setGetCallback(char *k, void (*cb)(class nlist *)) {
    bool failFlag=true;

    struct nlist *rec;

    rec=dbLookupRec( k ) ;
    if(rec == NULL) {
        failFlag=true;
    } else {
        rec->cmdGetCallback( cb );
    }

    return failFlag;
}

bool Small::setSubCallback(char *k, int (*cb)(class nlist *,uint8_t)) {
    bool failFlag=true;

    struct nlist *rec;

    rec=dbLookupRec( k ) ;
    if(rec == NULL) {
        failFlag=true;
    } else {
        rec->cmdSubscribeCallback( cb );
    }

    return failFlag;
}
/*
bool Small::setGlobalCallback( void (*cb)(class nlist *,uint8_t) ){
    bool failFlag=false;

    if( globalCallback != NULL ) {
        failFlag=true;
    } else {
        globalCallback = cb;
        failFlag = false;
    }

    return failFlag;
}
*/

void nlist::cmdSubscribeCallback(int (*func)(class nlist *,uint8_t)) {
    subscribeCallback = func;
}

void nlist::cmdSetCallback(void (*func)( class nlist *)) {
    setCallback=func;
}

void nlist::cmdGetCallback(void (*func)( class nlist *)) {
    getCallback=func;
}

struct nlist *nlist::getSectionNext() {
	return nextInSection;
}

bool nlist::getSection() {
	return sectionHead;
}

void nlist::setSection() {
	sectionHead=true;
}

void nlist::setSectionNext(struct nlist *rec) {
    nextInSection = rec;
}


struct nlist *Small::getFromFree() {
    class nlist *ptr;

    ptr = new nlist();

    return ptr;

    /*
    for(int i=0; i<HASHSIZE ;i++) {
        if (freeList[i] != NULL ) {
            ptr=freeList[i];
            freeList[i]=NULL;
            return(ptr);
        }
    }
    return(NULL);
    */
}

uint32_t Small::calcHashval(const char *n) {

    uint32_t hashval=0;

    for(hashval = 0; *n != '\0'; n++) {
        hashval = *n + 31 * hashval;
    }

    return (hashval % HASHSIZE);
}

bool Small::dbInstall(const char *n, const char *d) {
    bool rc=true; // Return true on error.
    int i;
    char *oldValue = NULL;

    nlist *v;

    v=dbLookupRec( n );

    if( v != NULL) {
    	oldValue = v->getDef();

    	// DOV-74 - Is the new value the same as the existing value ?
    	//          If so, no fault, just leave
    	if(!strcmp(oldValue,d)) {
    		rc=false;
    		return rc;
    	}
    }


#ifdef LINUX
    pthread_mutex_lock(&dbLock);
#endif
#ifdef FREERTOS
    osSemaphoreWait (dbLock, osWaitForever);
#endif
    if(v) {
    	rc=false;

        v->setDef( d );
 //       if(commsCallback) {

#ifdef LINUX
            printf("commsCallback set\n");
#endif

            for(i=0;i<MAX_SUB;i++) {
#ifdef LINUX
//                if( strlen(v->subscriber[i].name ) > 0) {
                if( v->subscriber[i].pipe  > 0) {
//                    rc=(*(commsCallback))(v->subscriber[i].name,cmdDef::SET, n, (char *)d) ;
                    rc=(*(commsCallback))(v->subscriber[i].pipe,cmdDef::SET, n, (char *)d) ;
                }
#else
                if( v->subscriber[i].pipe != 0) {
                    if( commsCallback != NULL) {
                        rc=(*(commsCallback))(v->subscriber[i].pipe,(char *)"SET", n, (char *)d) ;
                    }
                }
#endif
            }
    } else {

        v = getFromFree();

        int hashval = calcHashval( n ) ;

        v->setName( n );
        v->setDef( d );
        hash->add(hashval, v);
        rc=false;
    }

#ifdef LINUX
    pthread_mutex_unlock(&dbLock);
#endif
#ifdef FREERTOS
//    xSemaphoreGive(dbLock);
    osSemaphoreRelease(dbLock);
#endif
    return rc;  // TODO What causes an error ?
}

enum psError Small::unsubscribe( void *subscriber,const char *key){
    enum psError rc=PS_OK;

    nlist *v=dbLookupRec( key );

    // Not found so nothing to do.
    if( v == NULL) {
        rc=PS_OK;
        return rc;
    }

    if(v->amIPublished()) {
        rc=v->unsubscribe( subscriber );
        if( rc == PS_OK ) {
            subCount--;
            if(subCount <0) {
                subCount=0;
            }
#ifdef LINUX
            //            TODO Need to undestand why
            //            free(ret);
#endif
        }
    } else {
        // Not published, so nothing to do.
        rc=PS_OK;
        return rc;
    }
    return rc;
}


enum psError Small::subscribe( void *client,const char *key){
    enum psError rc=PS_OK;

    nlist *v=dbLookupRec( key );

    if( v == NULL ) {
        rc= PS_KEY_NOT_FOUND;
    } else {
        if(v->amIPublished()) {
#ifdef LINUX
            //            char *sid = (char *)malloc(MAX_SUB_NAME);
            //            char *tmp;
            //            if(sid) {
            //                rc=v->subscribe(client);
            //                strncpy(v->subscriber.client.name, (char *)client, MAX_SUB_NAME);
            //            }
#else
            //            void *sid =subscriber;
#endif
            //            rc=v->subscribe((void *)sid);
            rc=v->subscribe((void *)client);
            if(rc == PS_OK) {
                subCount++;
            }
        } else {
            rc=PS_NOSUB;
        }
    }

    return rc;
}

Small::~Small() {
    printf("Destroyed\n");
}
//
// nlist stuff
//

bool nlist::amIPublished() {
    return published;
}

void nlist::publish() {
    published=true;
}

void nlist::setReadOnly(bool ro) {
    readOnly=false;
}

bool nlist::getReadOnly() {
	return(readOnly);
}

void nlist::setConfidential(bool conf) {
    confidential = true;
}

bool nlist::getConfidential() {
	return confidential;
}
//
// TODO: Make this a virtual function.
//
bool nlist::alreadySubscribed(void *sid) {
    bool subscribed=false;

    for(int i=0; i < MAX_SUB;i++) {
#ifdef LINUX
        if(!strlen(subscriber[i].name) != 0) {
            if( !strcmp((char *)sid,(char *)subscriber[i].name)) {
                subscribed=true;
                break;
            }
        }
#else
        // TODO Fix doesn'check for already subscribed

         if ( sid == subscriber[i].pipe) {
            subscribed=true;
            break;
        }

#endif
    }
    return subscribed;
}

enum psError nlist::unsubscribe( void *sid) {
	int i;
	enum psError rc=PS_NOSUB;
	//    void *ret;

	for ( i=0;i<MAX_SUB;i++) {
#ifdef LINUX
		if( strlen(subscriber[i].name) != 0) {
			if(!strcmp((char *)sid, (char *)subscriber[i].name)) {
				if(subscriber[i].pipe != 0) {
#ifdef MQ
					mq_close(subscriber[i].pipe);
#endif
					subscriber[i].pipe=0;
				}
				rc=PS_OK;
				break;
			}
		}
#else
		if ( sid == subscriber[i].pipe) {
			subscriber[i].pipe=NULL;

			subCount--;
			if(subCount < 0 ) {
			    subCount=0;
			}
			rc=PS_OK;
			break;
		}
#endif
	}
	return rc;
}
// 
// TODO Sid is going to be the name.
//
enum psError nlist::subscribe( void *sid) {
    enum psError rc=PS_NOSUB;

    // TODO Scan the table first to see if already subscribed.
    if( alreadySubscribed(sid)) {
        rc=PS_RESUB;
        return rc;
    }
    for ( int i=0;i<MAX_SUB;i++) {
#ifdef LINUX
    	if(strlen(subscriber[i].name) == 0) {
            strncpy((char *)subscriber[i].name,(char *)sid,(unsigned int)SENDER_SIZE);
            rc = PS_OK;
            break;
        }
#else
    	if(subscriber[i].pipe == 0) {
    		subscriber[i].pipe = (QueueHandle_t)sid;
    		memset(&subscriber[i].name,0,sizeof(subscriber[i].name));
//            int err =(*(subscribeCallback))(this,i);

    		subCount++;
    		rc=PS_OK;
    		break;
    	}
#endif
    }

    return rc;
}

// void nlist::defaultSubCallback(class nlist *r,uint8_t idx) {     // This function performs the default subscribe action 
int nlist::defaultSubCallback(class nlist *rec, uint8_t idx) {               // This function performs the default subscribe action

	int channel=0;
#ifdef LINUX
    printf("defaultSubCallback fired\n");
#endif

    return channel;
}

/*
void nlist::defaultUnsubCallback(class nlist *r,uint8_t idx) {     // This function performs the default subscribe action 
#ifdef LINUX
    printf("defaultUnsubCallback fired\n");
#endif
}
*/

void nlist::commonInit() {

//    static bool done=false;

//    if(done == false ) {
//        done=true;
        next = NULL;
        subCount=0;
        nextInSection = NULL;
        sectionHead = false;

        memset(&name,0,MAX_NAME);
        memset(&defn,0,MAX_DEF);
        memset(&subscriber,0, sizeof(subscriber));

        published=false;		// If true other tasks can subscribe to this
        readOnly = false;		// If true other tasks can't set this.
        confidential = false; 	// If true other tasks can't see this.

        ttl = 0 ;                   // live forever.

        cbType = CB_FPTR;

//        subscribeCallback=&nlist::defaultSubCallback;
        subscribeCallback=NULL;
        unsubscribeCallback=NULL;
        setCallback=NULL;
        getCallback=NULL;
//    }
}

nlist::nlist() {
    commonInit();
}

nlist::nlist(const char *n, const char *d) {
    commonInit();

    strncpy(name, n, MAX_NAME);
    strncpy(defn, d, MAX_DEF);
}

void nlist::setName(const char *n) {
    strncpy(name, n, MAX_DEF);
}

char *nlist::getName() {
    return name;
}

uint8_t nlist::getSubCount() {
    return subCount;
}

class nlist *nlist::getNext() {
    return ( next);
}

bool nlist::setDef(const char *d) {
    bool rc=true; // fail by default.
    int i;
    // 
    // TODO Does this need changing to .....
    // ... callback gets called with the old and the new value ...
    // ... and returns the value to use.
    //
    if( strlen(name) > 0 ) {
        if( readOnly == false) {
            strncpy(defn,d,MAX_DEF);
        } else {
            //
            // if you attempt to update a readonly,
            // don't make a fuss, just don't do it.
            //
            rc=false;
            return rc;
        }

        // If there is a callback defined for the ...
        // ... SET opration, now is the time.
        //
        if( setCallback != NULL ) {
            // Do something on set
#ifdef LINUX
            printf("setCallback\n");
#endif
            (*(setCallback))(this);
        }
        // 
        // Action subscriber callback.
        //
        if(amIPublished()) {
        	switch( cbType ) {
        	case CB_INVALID:
        		rc=true;  // failed
        		break;
        	case CB_FPTR:
        	{
        		int len=0;
        		for(i=0;i<MAX_SUB;i++) {
        			len=strlen(subscriber[i].name);
        			if( len > 0) {
        				printf("execute comms calback here\n");
        			}
        		}
        	}
        	break;
        	case CB_XT:
        		// Currently unimplemented.
        		rc=true;
        		break;
        	}
        }
    } else {
    	rc=true; // it's all gone wrong
    }
    return rc;
}

struct client *nlist::getSubscriber(uint8_t idx) {
    struct client *ret=NULL;

    if(idx <= MAX_SUB) {
        ret=&subscriber[idx];
    }

    return ret;
}

char *nlist::getDef() {
    if(getCallback != NULL) {
        printf("getCallback set\n");
        (*(getCallback))(this);
    }

    return defn;
}

bool nlist::getDefBool() {

	bool res=false;


	res = (bool)(strcmp(defn,"YES") == 0) || (strcmp(defn,"ON")) || (strcmp(defn,"TRUE"));

	return res;
}


void nlist::setNext( class nlist *r) {
    next=r;
}
//
// 
// End of nlist
//
// Hashtab
//
hashtab::hashtab() {

    for(int i=0 ;i<HASHSIZE;i++) {
        bucket[i] = NULL;
    }
}

bool hashtab::add(const uint32_t n, class nlist *r) {
    bool rc=false;
    // nlist *ptr = NULL;

    if( bucket[n] == NULL) {
        bucket[n] = r;
    } else {
        r->setNext(bucket[n]);
        bucket[n]=r;
    }
    return rc;
}

class nlist *hashtab::getHead(int n) {
    return bucket[n];
}
// 
// Hashtab end
// 
// C Wrappers.
//
extern "C" struct Small *newSmall() {
    return new Small();
}

extern "C" bool addRecord(struct Small *db, const char *n, const char *v) {
    bool rc = false;

    rc = db->dbInstall(n,v);
    return rc;
}

#ifdef LINUX
extern "C" void display(struct Small *db) {
    db->printDb();
}
#endif

extern "C" char *dbLookup(struct Small *db, const char *n) {
    return db->dbLookup(n);
}

extern "C" struct nlist *dbLookupRec(struct Small *db, const char *n) {
    return db->dbLookupRec(n);
}

extern "C" bool dbPublish(struct Small *db,char *name) {

    bool rc=true;

    rc= db->publish(name);

    return rc;
}


extern "C" bool dbAmIPublished(struct nlist *rec) {
    return rec->amIPublished();
}

extern "C" bool setSetCallback(struct Small *db,char *k, void (*cb)(struct nlist *)) {
    bool ff=true;

    // TODO: Move this into the class method.
    //
    if ( db->dbLookup(k) == NULL) {
    	ff=db->dbInstall(k, "NOVALUE");
    } else {
    	// If I get here then the key exists, so all is good
    	//
    	ff = false;
    }

    if ( ff == false) {
    	ff=db->setSetCallback(k,cb);
    }

    return ff;
}

extern "C" bool setGetCallback(struct Small *db,char *k, void (*cb)(struct nlist *) ) {
    bool ff=true;

    if ( db->dbLookup(k) == NULL) {
    	ff=db->dbInstall(k, "NOVALUE");
    } else {
    	// If I get here then the key exists, so all is good
    	//
    	ff = false;
    }

    ff=db->setGetCallback(k,cb);

    return ff;
}

extern "C" bool setSubCallback(struct Small *db,char *k, int (*cb)(struct nlist *, uint8_t ) ) {
    bool ff=true;

    if ( db->dbLookup(k) == NULL) {
    	ff=db->dbInstall(k, "NOVALUE");
    } else {
    	// If I get here then the key exists, so all is good
    	//
    	ff = false;
    }

    ff=db->setSubCallback(k,cb);

    return ff;
}

extern "C" enum psError dbUnsubscribe(struct Small *db, void *sid, char *name) {
    enum psError rc=PS_OK;

    rc=db->unsubscribe(sid,name);
    return rc;
}

extern "C" enum psError dbSubscribe(struct Small *db, void *sid, char *name) {
    enum psError rc=PS_OK;

    rc=db->subscribe(sid,name);
    return rc;
}

extern "C" int32_t getSubCount(struct Small *db) {
    return db->getSubCount();
}

extern "C" char *nlistGetName(struct nlist *rec) {
    return rec->getName();
}

extern "C" char *nlistGetDef(struct nlist *rec) {
    return rec->getDef();
}

extern "C" bool nlistSetDef(struct nlist *rec, char *value) {
    return rec->setDef(value);
}

// See DOV-49

// extern "C" uint8_t nlistGetSubCount(struct nlist *rec) {
extern "C" uint8_t nlistGetSubCount(struct Small *db, char  *key) {
    struct nlist *rec;
    rec = db->dbLookupRec(key);
    return rec->getSubCount();
}

extern "C" void *nlistGetSubscriber(struct nlist *rec, uint8_t idx) {
    return rec->getSubscriber(idx);
}

extern "C" void dbDump(struct Small *db) {
	db->printDb();
}

extern "C" void dbIniDump(struct Small *db, char *path) {
	db->dumpIni(path);
}

extern "C" void setConfidential(struct nlist *rec, bool value) {
    rec->setConfidential(value);
}

extern "C" struct nlist *getSectionNext(struct nlist *rec) {
    return rec->getSectionNext();
}

extern "C" void setSectionNext( struct nlist *rec, struct nlist *link) {
    return rec->setSectionNext(link);
}

extern "C" bool getSection( struct nlist *rec) {
    return rec->getSection();
}

extern "C" void setSection(struct nlist *rec ) {
    rec->setSection();
}

extern "C" bool dbLookupBool(struct Small *db, char *name,bool dflt) {
	bool flag=false;
	flag =  db->dbLookupBool(name, dflt) ;
	return flag;
}
