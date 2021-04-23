#include <stdint.h>
#include <stdbool.h>
#include <string>

#ifndef __ENUMS
#define __ENUMS
enum cmdDef : uint8_t {
    NOP = 0,
    GET,
    SET,
    SUB,
    UNSUB,
    PING,
    PONG,
    EXIT,
    LAST_CMD
};

enum tasks : uint8_t {
    NOREPLY = 0,
    DBG = 1,
    TASK1,
    TASK2,
    TST,
    LAST_TASK
};

enum ptypes : uint8_t {
    INVALID=0,
    BOOL,
    BYTE,
    INT,            // 4 byte integer, lsb first
    STRING,
    JSON,
    LAST_TYPE
};
#endif
