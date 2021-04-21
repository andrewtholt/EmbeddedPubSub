/*
 * tasks.h
 *
 *  Created on: Nov 15, 2016
 *      Author: andrew.holt
 */

/**
 * @file tasks.h
 * @brief Test
 **/
#include "enums.h"

#ifndef MSGS_H_   //1
#define MSGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifdef LINUX
#ifdef MQ
#include <mqueue.h>
#endif
// #define SENDER_SIZE 32
#endif
#ifdef FREERTOS  // 2
// #include "cmsis_os.h"
#include "FreeRTOS.h"
#include "queue.h"
#endif //2

#define SENDER_SIZE 32

// 
// Make command an enum, or uint8_t
//
#define MAX_CMD 8       // Length of longest command i.e. UNSUB
#define MAX_KEY   16    // Length of longest key.
#define MAX_VALUE 32    // Length of longest value.

struct payloadCmd {
    uint8_t fields; // <= 3
    uint8_t cmd;
//    char cmd[MAX_CMD]; // TODO: change command from a string to an ENUM
    char key[MAX_KEY];
    char value[MAX_VALUE];
};

struct payloadFs {
    uint8_t fields; // <= 3
    uint8_t cmd;
//    char cmd[MAX_CMD];
    void *buffer;
    uint16_t bufferLength;
    int16_t fd;
};


#ifdef FREERTOS // 2
struct cmdMessage {
    QueueHandle_t sender;

    union {
    	struct payloadCmd message;
    	struct payloadFs messageFs;
    } payload;
};
#endif //2

#ifdef LINUX
struct cmdMessage {
    tasks sender;
//    char sender[SENDER_SIZE];
//    struct payload message;
    union {
        struct payloadCmd message;
    } payload;
};
#endif

#ifdef LINUX
struct client {
    char name[32];
#ifdef MQ
    mqd_t pipe;
#else
#ifdef SPREAD
    int pipe;
#endif
#endif
};
#endif

#ifdef FREERTOS
struct client {
    char name[32];
    QueueHandle_t pipe;
};
#endif

#ifdef __cplusplus
}
#endif

#endif

