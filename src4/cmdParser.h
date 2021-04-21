/**
 * 
 * @file cmdParser.h
 * @brief Header file.
 * @brief Message Parser
 */

#ifndef __CMDPARSER
#define __CMDPARSER
// #include "stm32f4xx_hal.h"
#ifdef FREERTOS
#include "cmsis_os.h"
#endif
/*
 */
/*
 * Pass a pointer to a variable of this type so that a command being parsed can
 * specify what should be done as a result of its action.
 */
enum postParseAction {
	PP_NULL=0,		// No further action.
	PP_FREE_MEM		// Release the memory used for the message back to the pool
};

typedef enum postParseAction postParseAction_t;

// #include "tasks.h"
#include "Small.h"
#include "enums.h"
struct parser;
#ifdef __cplusplus
// All virtual methods return a failure flag set to true until
// overridden.
//
/**
 * @class parser
 * @brief Take a command and parse it.
 */
class parser {
    protected:
        void commonInit();
        /**
         * @name commonInit()
         * @brief A common initialisation routine.
         * @ingroup example
         * _____
         */

        bool cmdPing(struct cmdMessage *,postParseAction_t *);

#ifdef LINUX
//        virtual bool fromMe(struct cmdMessage *, const char *) { return true ;}
        virtual bool fromMe(struct cmdMessage *, const tasks ) { return true ;}
#endif

#ifdef FREERTOS
        virtual bool fromMe(struct cmdMessage *, const QueueHandle_t ) { return true ;}
//        virtual bool fromMe(struct cmdMessage *, const char *) { return true ;}
#endif
        /**
         * @name fromMe
         * @brief Send a response message.
         * @ingroup example
         *
         * @param [in] struct cmdMessage *
         * @param [in] char *recipient
         *
         * @retval TRUE     Send failed.
         * @retval FALSE    Send succeeded.
         *
         * _____
         */
        //        virtual bool cmdSub(struct cmdMessage *) { return true ; }
        bool cmdSub(struct cmdMessage *,postParseAction_t *);
        /**
         * @name cmdSub
         * @brief Subscribe to a value.
         * @ingroup parser
         *
         * @param [out] Pointer to cmdMessage
         * @param [in]  Name of value to subscribe to.
         *
         * @retval TRUE     Subscribe failed.
         * @retval FALSE    Subscribe  succeeded.
         *
         * _____
         */
        bool cmdUnsub( cmdMessage *,postParseAction_t *);
        /**
         * @name cmdUnsub
         * @brief Subscribe to a value.
         * @ingroup parser
         *
         * @param [out] Pointer to cmdMessage
         * @param [in]  Name of value to subscribe to.
         *
         * @retval TRUE     Subscribe failed.
         * @retval FALSE    Subscribe  succeeded.
         *
         * _____
         */
    private:
        bool dontCreate;  // If this is true then SET will only update existing values.
        bool cmdOpen(struct cmdMessage *, postParseAction_t *);

        // 
        // Pass in a pointer to structure holding contact information and ...
        // The variable to subscribe to.
        // 
        bool cmdSet(struct cmdMessage *, postParseAction_t *);
        /**
         * @name cmdSet
         * @brief Set a value.
         * @ingroup parser
         *
         * @param [in] name
         * @param [in] value
         *
         * @retval TRUE     Set value failed.
         * @retval FALSE    Set value succeeded.
         *
         * _____
         */
    public:
        class Small *data;

        parser(void);
        /**
         * @name parser
         * @brief Create an instance
         * @ingroup example
         */
        parser(class Small *);
        void setDontCreate() ;
        /**
         * @name setDontCreate()
         *
         * @brief Set the don't create flag.  
         *
         * The effect of this is that if a SET command is received for a variable that does 
         * not exist it will not be created.
         *
         * _____
         */

        class Small *getDb();
        /**
         * @name getDb
         * @brief Get a pointer to the Small database in use
         *
         * @retval Pointer to an instance of Samll.
         *
         * _____
         */
        void setDb(Small *db);
        /**
         * @name setDb
         * @brief Set a pointer to the Small database to use.
         *
         * @param [in] db  Pointer to an instance of Small.
         *
         * This allows a task (say the atlast interpreter) to connect to any tasks
         * database.  This is not to be abused, it is for debugging only.
         * _____
         */


        bool parse(struct cmdMessage *msg, postParseAction_t *act);
        /**
         * @name parse
         * @brief Parse a received message.
         *
         * @param [in] msg Pointer to a message.
         * @retval TRUE Failed to parse message.
         *
         * _____
         */
        ~parser();
        virtual char *version() { return (char *)"Version:cmdParser"; }
        /* 
         *  Default behaviour is that overridden methods fail until overridden.
         */ 

        virtual bool cmdGet(struct cmdMessage *, postParseAction_t *);
        /**
         * @name cmdGet
         * @brief Get a value.
         * @ingroup parser
         *
         * @param [out] Pointer to cmdMessage
         * @param [in]  Name of value to get.
         *
         * @retval TRUE     Get value failed.
         * @retval FALSE    Get value succeeded.
         *
         * _____
         */
        // virtual bool cmdGet(struct cmdMessage *) { return true ; }
};
#endif

#ifdef __cplusplus
extern "C" {
    struct Small;
#endif
    //    struct parser *newParser() ;
    struct Small *parserGetDb(struct parser *) ;
    /*
     * @name parserGetDb
     * @brief Get database.
     * @param[in] Pointer to a parser class instance.
     */
    bool parserParse(struct parser *, struct cmdMessage *);
    void parserSetDontCreate(struct parser *);
#ifdef __cplusplus
}
#endif

#endif
