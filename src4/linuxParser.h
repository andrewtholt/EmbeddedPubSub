
/**
 * @file linuxParser.h
 * @brief Header file.
 */

#ifndef __LINUX_PARSER
#define __LINUX_PARSER

#include <mqueue.h>
#include "cmdParser.h"
struct linuxParser;

#ifdef __cplusplus    // C++
#include <iostream>
using namespace std;

class linuxParser: public  parser {

    private:
        virtual bool fromMe(struct cmdMessage *,const mqd_t );
        /**
         * 
         * @name fromMe
         * @brief Send a message.
         *
         * @param [in] msg Pointer to completed message structure.
         * @param [in] recipient Name of message recipient.
         *
         * @retval TRUE Message send failed..
         * @retval FALSE Message sent succesfully.
         */
    protected:
        mqd_t iam;
    public:
        linuxParser();
        linuxParser(Small *);

        void setIam(mqd_t);
        mqd_t getIam();

        /**
         * @name commsCallback
         * @brief Send the given message to the recipient identified by name.
         */
        virtual char *version();
//        static bool commsCallback(const char *dest, const char *cmd, const char *key, const char *value);
        static bool commsCallback(const mqd_t dest, const enum cmdDef c, const char *key, const char *value);
        /**
         * @name commsCallback
         * @brief Send the given message to the recipient identified by name.
         *
         * @param [in] Pointer to completed message structure.
         * @param [in] Name of message recipient.
         *
         * @retval TRUE Message send failed..
         * @retval FALSE Message sent succesfully.
         */
};

#else  // Not C++
// For C programs
//
/**
 * @name linuxParser
 * @brief Return an instance of the cmdParser.
 *
 */
extern struct linuxParser *newParser();
extern void setIam();
extern bool cmdParse();


#endif
#endif

