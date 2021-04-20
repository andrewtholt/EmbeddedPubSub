#include <stdint.h>
#include <stdbool.h>
#include <string>

enum cmd : uint8_t {
    NOP = 0,
    GET,
    SET,
    SUB,
    UNSUB,
    PING,
    EXIT,
    LAST_CMD
};

enum tasks : uint8_t {
    DBG = 0,
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

