#include "proto.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../KVStore/kvs.h"
#include "pubSub.h"

extern KVSstore *store;

char *printCmd(enum cmd c) {

    char res[6] = {0};

    switch(c) {
        case NOP:
            strcpy(res,"NOP");
            break;
        case GET:
            strcpy(res,"GET");
            break;
        case SET:
            strcpy(res,"SET");
            break;
        case SUB:
            strcpy(res,"SUB");
            break;
        case UNSUB:
            strcpy(res,"UNSUB");
            break;
        case PING:
            strcpy(res,"PING");
            break;
        case EXIT:
            strcpy(res,"NEXT");
            break;
        default:
            strcpy(res,"WHAT?");
            break;
    }
}

void mkGetStringCmd(char *key, uint8_t *out) {
    uint8_t size=0;
    uint8_t idx=1;

    memset(out,0,MAX_PACKET);

    out[idx++] = SET;
    out[idx++] = strlen(key);

    memcpy(&out[idx], key, strlen(key));

    idx += strlen(key);

    out[idx] = STRING;
    out[0] = idx;
}

void mkSetStringCmd(char *key, char *state, uint8_t *out) {
    uint8_t size=0;
    uint8_t idx=1;

    memset(out,0,MAX_PACKET);

    out[idx++] = SET;
    out[idx++] = 0; // Sender N/A for set
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

void mkGetIntCmd(char *key, uint8_t *out) {
    uint8_t size=0;
    uint8_t idx=1;

    memset(out,0,MAX_PACKET);

    out[idx++] = SET;
    out[idx++] = strlen(key);
    memcpy(&out[idx], key, strlen(key));

    idx += strlen(key);

    out[idx] = INT;
    out[0] = idx;
}

void mkSetIntCmd(char *key, uint32_t state, uint8_t *out) {
    uint8_t size=0;
    uint8_t idx=1;

    memset(out,0,MAX_PACKET);

    out[idx++] = SET;
    out[idx++] = 0; // Sender N/A for set
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

void mkGetByteCmd(char *key, uint8_t *out) {
    uint8_t size=0;
    uint8_t idx=1;

    memset(out,0,MAX_PACKET);

    out[idx++] = SET;
    out[idx++] = strlen(key);
    memcpy(&out[idx], key, strlen(key));
    idx += strlen(key);

    out[idx] = BYTE;

    out[0] = idx;
}

void mkSetByteCmd(char *key, uint8_t state, uint8_t *out) {
    uint8_t size=0;
    uint8_t idx=1;

    memset(out,0,MAX_PACKET);

    out[idx++] = SET;
    out[idx++] = 0; // Sender N/A for set
    out[idx++] = strlen(key);
    memcpy(&out[idx], key, strlen(key));
    idx += strlen(key);

    out[idx++] = 2;
    out[idx++] = BYTE;

    out[idx] = state;

    printf("len = %d\n", idx);
    out[0] = idx;
}

void mkGetBoolCmd(char *key, uint8_t *out) {
    uint8_t size=0;
    uint8_t idx=1;

    memset(out,0,MAX_PACKET);

    out[idx++] = GET;
    out[idx++] = strlen(key);

    memcpy(&out[idx], key, strlen(key));
    idx += strlen(key);
    out[idx] = BOOL;

    out[0] = idx;
}

void mkSetBoolCmd(char *key, bool state, uint8_t *out) {
    uint8_t size=0;
    uint8_t idx=1;

    memset(out,0,MAX_PACKET);

    out[idx++] = SET ;
    out[idx++] = 0; // Sender N/A for set
    out[idx++] = strlen(key);

    memcpy(&out[idx], key, strlen(key));
    idx += strlen(key);

    out[idx++] = 2;
    out[idx++] = BOOL;

    out[idx] = (state == true) ? 1 :0 ;

    printf("len = %d\n", idx);

    out[0] = idx;

}

// 
// If you know that this command has no return (SET, EXIT) then out_ptr may be NULL
// Since it will not be referenced
//
void interpPacket(uint8_t *in_ptr, uint8_t *out_ptr) {

    uint8_t idx = 0;
    uint8_t packetLen = in_ptr[idx++];
    uint8_t cmd = in_ptr[idx++];

    printf("Command  :%d %s\n",cmd,printCmd(cmd));

    switch(cmd) {
        case SET:
            interpSetPacket(in_ptr);
            break;
        case GET:
            break;
    }
//    printf("data type:%d\n",dataType);
}

void interpGetPacket(uint8_t *in_ptr, uint8_t *out_ptr) {

    uint8_t idx = 0;
    uint8_t packetLen = in_ptr[idx++];
    uint8_t cmd = in_ptr[idx++];

    uint8_t key[8] = { 0 };

    printf("Command  :%d %s\n",cmd,printCmd(cmd));

    if ( cmd != GET) {
        return;
    }

    printf("idx=%d\n", idx);

    uint8_t keyLength = in_ptr[idx++];

    memcpy(key, &in_ptr[idx], keyLength);

    idx += keyLength;
}

void interpSetPacket(uint8_t *ptr) {
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

    printf("Command  :%d %s\n",cmd,printCmd(cmd));
    printf("data type:%d\n",dataType);

    // TODO: Each case would write to the database not yet built.
    switch(dataType) {
        case(BOOL):
            {
                printf("Set bool %s to %d\n",key, ptr[idx]);

                bool state = (ptr[idx] != 0) ? true : false;

                setBoolean(store, key, state);

            }
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

