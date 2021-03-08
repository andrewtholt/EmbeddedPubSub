#include <stdint.h>
#include <stdbool.h>

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

#define MAX_PACKET 32

void interpPacket(uint8_t *ptr);

void mkBoolCmd(char *key, bool state, uint8_t *out);
void mkByteCmd(char *key, uint8_t state, uint8_t *out);
void mkIntCmd(char *key, uint32_t state, uint8_t *out);
void mkStringCmd(char *key, char *state, uint8_t *out);

