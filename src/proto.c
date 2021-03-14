#include "proto.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../KVStore/kvs.h"

extern KVSstore *store;

void mkStringCmd(char *key, char *state, uint8_t *out) {
    uint8_t size=0;
    uint8_t idx=1;

    memset(out,0,MAX_PACKET);

    out[idx++] = SET;
    out[idx++] = strlen(key);

    memcpy(&out[idx], key, strlen(key));

    idx += strlen(key);

    out[idx++] = strlen(state);
    out[idx++] = STRING;

    uint8_t stateLen = strlen(state);
    printf("stateLen = %d\n", stateLen);
    out[idx++] = stateLen;
    memcpy(&out[idx], state, strlen(state));
    idx += stateLen;

    printf("len = %d\n", idx);
    out[0] = idx;
}

void mkIntCmd(char *key, uint32_t state, uint8_t *out) {
    uint8_t size=0;
    uint8_t idx=1;

    memset(out,0,MAX_PACKET);

    out[idx++] = SET;
    out[idx++] = strlen(key);
    memcpy(&out[idx], key, strlen(key));

    idx += strlen(key);

    out[idx++] = sizeof(uint32_t);
    out[idx++] = INT;

    memcpy(&out[idx], &state, sizeof(state));

    idx += sizeof(state);

    printf("len = %d\n", idx);
    out[0] = idx;
}

void mkByteCmd(char *key, uint8_t state, uint8_t *out) {
    uint8_t size=0;
    uint8_t idx=1;

    memset(out,0,MAX_PACKET);

    out[idx++] = SET;
    out[idx++] = strlen(key);
    memcpy(&out[idx], key, strlen(key));
    idx += strlen(key);

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

    printf("data type %d\n",dataType);

    // TODO: Each case would write to the database not yet built.
    switch(dataType) {
        case(BOOL):
            printf("Set bool %s to %d\n",key, ptr[idx]);
            break;
        case(BYTE):
            printf("Set byte %s to 0x%02x\n",key, ptr[idx]);
            break;
        case(INT):
            {
                uint32_t tmp=0;
                printf("len = %d\n", (ptr[idx-1])-1);
                memcpy(&tmp, &ptr[idx], 4);
                printf("Set int %s to 0x%04x\n",key, tmp);
            }
            break;
        case(STRING):
            {
                uint8_t dataLen=0;
                printf("idx = %d\n", idx);
                dataLen = ptr[idx] ;
                printf("len = %d\n", dataLen);
                idx++;

                uint8_t tmp[32]={ 0 }; // Guess
                strncpy(tmp,&ptr[idx], dataLen);
                printf("Set string %s to %s\n",key, tmp);
            }

            break;
    }
    return;
}

