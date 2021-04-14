/**
 * @file Small.h
 * @brief Header file.
 */
#ifndef __SMALL_H
#define __SMALL_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef LINUX
#include <pthread.h>
#endif

#include "msgs.h"

#define MAX_NAME 32
#define MAX_DEF 32
#define MAX_REC 32
#define HASHSIZE 5
#define MAX_SUB 16	// Maximum number of subscribers
#define MAX_SUB_NAME 32
#ifdef FREERTOS
#include "cmsis_os.h"
#include <semphr.h>

#endif

enum cbTypeList {
    CB_INVALID=0,   // Invalid or undefined, so nothing happens
    CB_FPTR,        // Callback is a C/C++ function
    CB_XT           // Callback is a Atlast execution token.
};

enum psError {
	PS_OK=0,
	PS_KEY_NOT_FOUND,
	PS_NOSUB,
	PS_RESUB
};

/*
struct subscription {
    char name[MAX_SUB_NAME];
    int pipe;
};
*/
// TODO Investigate having a private method named callBack that is overloaded.
// This would remove setGlobalCallback and the associated private variable.
//
// Customisation would be by subclassing Small.
//
struct Small;
struct nlist;

#ifdef __cplusplus
class Small {
    private:
		struct nlist *freeList[MAX_REC];
//        class nlist **freeList;
        class hashtab *hash;
        uint32_t calcHashval(const char *n);
        struct nlist *getFromFree();
//        void (*globalCallback)(class nlist *,uint8_t);  // TODO Only used by dbInstall

        // dest & cmd MUST be set to point to a string.
        // key and value may be NULL;
#ifdef LINUX
        bool (*commsCallback)( const char *dest, const char *cmd, const char *key, const char *value) ;
#else
        bool (*commsCallback)( const QueueHandle_t dest, const char *cmd, const char *key, const char *value) ;
#endif
        int32_t subCount;
#ifdef LINUX
        pthread_mutex_t dbLock;
#endif

#ifdef FREERTOS
        SemaphoreHandle_t dbLock;
#endif

    protected:
        friend class cmdParser;
    public:
        Small();
        /**
         * @name Small
         * @brief Create an empty database.
         * ________________________________________________________________________
         */

        int32_t getSubCount();

        void incSubCount();
        void decSubCount();

        bool publish(char *);
        /**
         * @name publish
         * @brief Set the published flag of the record with the given key.
         *
         * @param [in] char *name
         * @retval TRUE Record does not exist.
         * @retval FALSE Record exists and has been subscribed to.
         *
         * ________________________________________________________________________
         *
         */
        bool readOnly(char *,bool );
        /**
         * @name readOnly
         * @brief Set the readonly flag.
         *
         * @param[in] char pointer to the name of record.
         * @param[in] Required state.
         *
         * @retval Failure flag.
         *
         * If the readonly flag is set to true a SET will not change the value.
         *
         * ________________________________________________________________________
         */
        bool confidential(char *n,bool );
        /**
         * @name confidential
         * @brief Set the readonly flag.
         *
         * @param[in] char pointer to the name of record.
         * @param[in] Required state.
         *
         * If the confidential flag is set to true access is given ONLY to the task owning the data.
         *
         * ________________________________________________________________________
         */
        class nlist *dbLookupRec(const char *n);  // Return the record instance for key.
        /**
         * @name dbLookupRec
         * @brief Return a pointer to record with the given name.
         * @param[in] n char pointer to the name of record.
         *
         * @retval pointer to an nlist instance.
         *
         * Returns NULL if the record does not exist.
         * If successful returns an nlist class instance pointer to the record:
         * ________________________________________________________________________
         */
        //
        // TODO Are these constructors used.
        //
        Small(const int h);   // Pass in hash size
        Small(const int h, const int m); // Pass in hash size, and max records

        void dumpIni(char *path) ;
        void printDb() ;
        bool dbInstall(const char *n, const char *d);
        /**
         * @name dbInstall
         * @brief Add a record to the database, or update an existing record.
         *
         * @param[in] n Pointer to a record name
         * @param[in] d Pointer to a record value
         *
         * @retval Failure flag.
         *
         * If the return is true, the operation failed.
         *
         * ________________________________________________________________________
         */
        char *dbLookup(const char *n);
        /**
         * @name dbLookup
         * @brief Search for a record in the database.
         *
         * @param[in] n Pointer to a record name
         * @retval Pointer to value.
         *
         * ________________________________________________________________________
         */
        bool dbLookupBool(char *name);
        bool dbLookupBool(char *name, bool dflt);

#ifdef LINUX
        bool setCommsCallback(bool (*cb)(const char *, const char *, const char *, const char *));
#else
        bool setCommsCallback(bool (*cb)(const QueueHandle_t , const char *, const char *, const char *));
#endif
        bool setSetCallback(char *k, void (*cb)(class nlist *));
        /**
         * @name setSetCallback
         * @brief Set the function to call when a SET command is executed.
         *
         * @param[in] k Pointer to record name
         * @param[in] cb Pointer to function.
         *
         * @retval Failure flag.
         * If the return is true, the operation failed.
         *
         * ________________________________________________________________________
         */
        bool setGetCallback(char *k, void (*cb)(class nlist *));
        /**
         * @name setGetCallback
         * @brief Set the function to call when a GET command is executed.
         *
         * @param[in] k Pointer to record name
         * @param[in] cb Pointer to function.
         *
         * @retval Failure flag.
         * If the return is true, the operation failed.
         *
         * ________________________________________________________________________
         */

        enum psError subscribe( void *sid,const char *key);
        /**
         * @name subscribe
         * @brief Subscribe to the record indicated by the given key.
         *
         * @param[in] sid The idenity of the subscriber.
         * @param[in] key Pointer to a key.
         *
         * @retval An error indicator.
         *
         * The idetity of the sender is actually that of a communication channel back to the subscriber.  This is system dependent and so could be a FreeRTOS queue, or a Unix file descriptor.  The onus is on the the subscriber to understand what this value means in its context.
         *
         * ________________________________________________________________________
         */
        enum psError unsubscribe( void *sid,const char *key);
        
        bool setGlobalCallback( void (*cb)(class nlist *,uint8_t) );  // 
        bool setSubCallback(char *k, int (*cb)(class nlist *, uint8_t));    // Set SUBSCRIBE callback in records
        bool setUnsubCallback(char *k, void (*cb)(class nlist *,uint8_t));  // Set UNSUBSCRIBE callback in records

        ~Small();
};

class nlist {
    private:
        class nlist *next;
        class nlist *nextInSection;
        bool sectionHead;

        char name[MAX_NAME];
        char defn[MAX_DEF];
        void commonInit();
        uint8_t subCount;  // The number of subscribers.

        bool published;
        // These are for the benefit of the command parser ...
        // .. if readOnly is true it will silently drop and SET commands ...
        // .. If confidential is true it will return NOVAR to GET and ...
        // .. Ignore SET.
        // 
        bool readOnly;      
        bool confidential;
        // TODO add a timestamp.
        uint16_t ttl;	// If zero live forever, otherwise if not update in atleast this time
        				// replace the value with the string "NODATA"

        // This function performs the default action 
        //
        static void defaultCallback(class nlist *,uint8_t );
        //
        // This function performs the default subscribe action 
        //
        static int defaultSubCallback(class nlist *, uint8_t );
        //
        // This function performs the default unsubscribe action.
        //
        static void defaultUnsubCallback(class nlist *,uint8_t );
        //
        // Pass in name of channel, set it up and return a handle.
        //
        int (*subscribeCallback)(class nlist *,uint8_t );
        // 
        // Tear down the previously set up channel.
        //
        void (*unsubscribeCallback)(char *);

        void (*setCallback)(class nlist *);
        void (*getCallback)(class nlist *);
    public:
        nlist();
        nlist(const char *n, const char *d);

        void publish();
        uint8_t getSubCount();
        void setReadOnly(bool );
        bool getReadOnly( );

        void setConfidential(bool );
        bool getConfidential( );

        bool getSection();
        void setSection();

        void setSectionNext(nlist *ptr);
        nlist *getSectionNext();
    /**
     * @name    alreadySubscribed
     * @brief   Check if a subscriber is already subscribed.
     *
     * @param [in] (void *) sender Id.
     *
     * @retval  TRUE 	Parameter is subscribed to already.
     * @retval  FALSE	Parameter is not subscribed to.
     * ________________________________________________________________________
     *
     */
        bool alreadySubscribed(void *);

        enum cbTypeList cbType;
        // TODO Implement these.
        enum cbTypeList setCbType;
        enum cbTypeList getCbType;
        enum cbTypeList subCbType;

        struct client subscriber[MAX_SUB];  // This will hold the identifier of the subscriber.

        void setName(const char *);
        char *getName();
        char *getDef();
        bool getDefBool();
        
        struct client *getSubscriber(uint8_t);
        bool setDef(const char *);
        void setNext(class nlist *);
        class nlist *getNext();
        bool amIPublished();

        void cmdSubscribeCallback(int (*func)( class nlist *, uint8_t ));   // Set the callback for SUB
        void cmdUnsubscribeCallback(void (*func)( char * ));    // Set the vallback for UNSUB
        
        void cmdSetCallback(void (*func)( class nlist *));  // Set the callback for SET
        void cmdGetCallback(void (*func)( class nlist *));  // Set the callback for GET

        enum psError subscribe(void *);
        enum psError unsubscribe(void *);
        
};

class hashtab {
    private:
        class nlist *bucket[HASHSIZE] ;
    public:
        hashtab();
        hashtab(int size);
        bool add(const uint32_t n, class nlist *r);
        class nlist *getHead(int);
};



extern "C" {
#else
    struct Small;
    struct nlist;

#endif
    struct Small *newSmall();
    /**
     * @name    newSmall
     * @brief   Interface for C to create new instance of class Small.
     * @ingroup example
     *
     * @param [in] None.
     *
     * @retval  Pointer to structure Small
     *
     * Example Usage:
     * @code
     *    struct Small *db = newSmall();
     * @endcode
     *
     */


    bool addRecord(struct Small *db, const char *n, const char *v);
    /**
     * @name    addRecord
     * @brief   Add a parameter to the pub/sub system.
     * @ingroup example
     *
     * @param [in] db   Pointer to struct Small.
     * @param [in] n    Pointer to parameter name.
     * @param [in] v    Pointer to parameter value.
     *
     * @retval  Pointer to structure Small
     *
     * Example Usage :
     * @code
     *    bool failFlag = addRecord(db,n,v);
     * @endcode
     */
    char *dbLookup(struct Small *db, const char *n);
    /**
     * @name    dbLookup
     * @brief   Search for a key name and return the value.
     * @ingroup example
     *
     * @param [in] db   Pointer to struct Small.
     * @param [in] n    Pointer to parameter name.
     *
     * @retval  Boolean indicating failure.
     *
     * Example Usage:
     * @code
     *    char *value = addRecord(db,n);
     * @endcode
     */
    bool dbPublish(struct Small *db, char *n);
    /**
     * @name    dbPublish
     * @brief   Set a paramater to subscribable.
     * @ingroup example
     *
     * @param [in] db   Pointer to struct Small.
     * @param [in] n    Pointer to parameter name.
     *
     * @retval  Boolean indicating failure.
     *
     * Example Usage:
     * @code
     *    bool failFlag = dbPublish(db,n);
     * @endcode
     */
    bool dbPublished(struct Small *db, char *n);
    /**
     * @name    dbPublished
     * @brief   Return a flag indicating if the paramter is published
     * @ingroup example
     *
     * @param [in] db   Pointer to struct Small.
     * @param [in] n    Pointer to parameter name.
     *
     * @retval  TRUE 	Parameter is published, i.e. can be subscribed to.
     * @retval  FALSE	Parameter is unpublished, i.e. private.
     *
     * Example Usage:
     * @code
     *    bool failFlag = dbPublished(db,n);
     * @endcode
     */
    void cmdSubscribeCallback(struct Small *db, const char *,void (*cb)(struct nlist *,uint8_t));
    /**
     * @name    cmdSubscribeCallback
     * @brief   Set callback on subscribe function.
     * @ingroup example
     *
     * @param [in] db   Pointer to struct Small.
     * @param [in] ptr  Pointer to key string.
     * @param [in] func Pointer to function.
     *
     * When a parameter has been published a list of subscribers is iterated over.  For each subscriber the callback is executed.
     * The function prototype is:
     * @code
     * 	    void cmdSubscribeCallback(struct Small *,const char *, void (*cb)(struct nlist *,uint8_t));
     * @endcode
     *
     * Example Usage:
     * @code
     *    cmdSubscribeCallback(db,"TEST", func);
     * @endcode
     */

    bool setGlobalCallback(struct Small *db , void (*cb)(struct nlist *,uint8_t) ) ;
    /**
     * @name    setGlobalCallback
     * @brief   Set callback function.
     * @ingroup example
     *
     * @param [in] db   Pointer to struct Small.
     * @param [in] func Pointer to function
     *
     * Sets a table wide callback function.  Is set each record, when created, inherits ths value.  If not set then a default is used.
     * n individual record can have its callback reset by using setCallback.
     * 
     * The function prototype is:
     * @code
     * 	    void setGlobalCallback(struct Small *, void (*cb)(struct nlist *,uint8_t) )
     * @endcode
     *
     * Example Usage:
     * @code
     *    db=newSmall();
     *    setGlobalCallback(db,func);
     * @endcode
     */

    enum psError dbSubscribe(struct Small *, void *, char *);
    /**
     * @name dbSubscribe
     * @brief Subscribe client to a value in the database.
     */

    char *nlistGetName(struct nlist *);
#ifdef LINUX
    void display(struct Small * );
#endif
    /**
     * @name printBoolean
     * @brief Take a true/false boolean and print TRUE or FALSE
     */
    void printBoolean(bool v);
#ifdef __cplusplus
}
#else
extern struct Small *newSmall() ;
extern bool addRecord(struct Small *db, const char *n, const char *v) ;
extern void display(struct Small *db) ;
extern char *dbLookup(struct Small *db, const char *n) ;
extern struct nlist *dbLookupRec(struct Small *db, const char *n) ;
extern bool dbPublish(struct Small *db,char *name) ;
extern bool dbAmIPublished(struct nlist *rec) ;
extern bool setSubCallback(struct Small *,char *, int (*cb)(struct nlist *, uint8_t ));
extern bool setSetCallback(struct Small *,char *, void (*cb)(struct nlist *));
extern bool setGetCallback(struct Small *,char *, void (*cb)(struct nlist *));
extern enum psError dbUnsubscribe(struct Small *db, void *sid, char *name);
extern enum psError dbSubscribe(struct Small *db, void *sid, char *name);
extern char *nlistGetName(struct nlist *rec);
extern char *nlistGetDef(struct nlist *rec);
extern bool nlistSetDef(struct nlist *rec, char *value) ;
extern uint8_t nlistGetSubCount(struct Small *db, char  *key);
extern void *nlistGetSubscriber(struct nlist *rec, uint8_t idx);

extern void setConfidential(struct nlist *rec, bool value);
extern void getSection(struct nlist *rec);
extern void setSection();
#endif

#endif // __SMALL_H
