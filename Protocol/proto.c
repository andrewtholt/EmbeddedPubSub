#include "proto.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void mkByteCmd(char *key, uint8_t state, uint8_t *out) {
    uint8_t size=0;
    uint8_t idx=1;

    memset(out,0,MAX_PACKET);

    out[idx++] = SET;
    out[idx++] = strlen(key);
    memcpy(&out[idx], key, strlen(key));

    out[idx++] = 2;
    out[idx++] = BYTE;

    out[idx] = state;

    printf("len = %d\n", idx);
    out[0] = idx;
}


void mkBoolCmd(char *key, bool state, uint8_t *out) {
    uint8_t size=0;
    uint8_t idx=1;

    memset(out,0,MAX_PACKET);

    out[idx++] = SET;
    out[idx++] = strlen(key);

    memcpy(&out[idx], key, strlen(key));
    idx += strlen(key);
    out[idx++] = 2;
    out[idx++] = BOOL;

    out[idx] = (state == true) ? 1 :0 ;

    printf("len = %d\n", idx);

    out[0] = idx;

}

void interpPacket(uint8_t *ptr) {
    uint8_t idx = 0;
    uint8_t packetLen = ptr[idx++];
    uint8_t cmd = ptr[idx++];

    uint8_t key[8] = { 0 };

    uint8_t i = 0;
    for(i = 0; i < LAST_CMD; i++) {
        printf("i=%d\n", i);

        if( cmd == i) {
            break;
        }
    }

    printf("idx=%d\n", idx);

    uint8_t keyLength = ptr[idx++];

    memcpy(key, &ptr[idx], keyLength);

    idx += keyLength;
    uint8_t dataLen = ptr[idx++];

    uint8_t dataType = ptr[idx++];

    switch(dataType) {
        case(BOOL):
            printf("Set bool %s to %d\n",key, ptr[idx]);
            break;
        case(BYTE):
            printf("Set byte %s to 0x%02x\n",key, ptr[idx]);
            break;
    }
    return;
}

