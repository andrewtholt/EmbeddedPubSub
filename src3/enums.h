#include <stdint.h>
#include <stdbool.h>
#include <string>

enum cmd {
    NOP = 0,
    GET,
    SET,
    SUB,
    UNSUB,
    PING,
    EXIT,
    LAST_CMD
};

enum tasks {
    DBG = 0,
    TST,
    LAST_TASK
};

enum ptypes {
    INVALID=0,
    BOOL,
    BYTE,
    INT,            // 4 byte integer, lsb first
    STRING,
    JSON,
    LAST_TYPE
};

