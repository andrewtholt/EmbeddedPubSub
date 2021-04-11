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

#define MAX_PACKET 32

void interpPacket(uint8_t *in_ptr, uint8_t *out_ptr);
void interpSetPacket(uint8_t *ptr);

void mkGetBoolCmd(uint8_t sender, char *key, uint8_t *out);
void mkSetBoolCmd(char *key, bool state, uint8_t *out);

void mkGetByteCmd(uint8_t sender, char *key, uint8_t *out) ;
void mkSetByteCmd(char *key, uint8_t state, uint8_t *out);

void mkGetIntCmd(uint8_t sender, char *key, uint8_t *out) ;
void mkSetIntCmd(char *key, uint32_t state, uint8_t *out);

void mkGetStringCmd(uint8_t sender, char *key, uint8_t *out);
void mkSetStringCmd(char *key, char *state, uint8_t *out);

