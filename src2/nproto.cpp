/***********************************************************************
 * AUTHOR: Andrew Holt <andrewh>
 *   FILE: .//nproto.cpp
 *   DATE: Sat Apr 10 16:23:25 2021
 *  DESCR: 
 ***********************************************************************/
#include "nproto.h"
#include <cstdio>
#include <string>
#include <iostream>
#include "dump.h"

/***********************************************************************
 *  Method: proto::db
 *  Params: 
 * Effects: 
 ***********************************************************************/
proto::proto() {
    printf("Create proto\n");
//    store = kvs_create(strcmp);
//    Use the db class instead.
}


/***********************************************************************
 *  Method: proto::interpPacket
 *  Params: uint8_t *in_ptr, uint8_t *out_ptr
 * Returns: void
 * Effects: 
 ***********************************************************************/
void proto::interpPacket(uint8_t *in_ptr, uint8_t *out_ptr) {
    uint8_t idx = 0;
    uint8_t packetLen = in_ptr[idx++];
    uint8_t c = in_ptr[idx++];

    cmd castEnum = static_cast<cmd>(c);

    std::cout << printCmd(castEnum) << "\n";

    switch(c) {
        case SET:
            interpSetPacket(in_ptr, packetLen);
            break;
        case GET:
            break;
    }

}


/***********************************************************************
 *  Method: proto::interpSetPacket
 *  Params: uint8_t *ptr
 * Returns: void
 * Effects: 
 ***********************************************************************/
void proto::interpSetPacket(uint8_t *ptr, uint8_t len) {
    char key[8] = {0};
    uint8_t idx=3;
    uint8_t klen = 0;
    uint8_t dlen = 0;

    dump(ptr,len);

    klen = ptr[idx++];
    std::cout << "Key Len:" << std::to_string(klen ) << "\n";

    memcpy(key, &ptr[idx], klen);
    std::cout << "Key    :" << key << "\n";


    idx += klen;

    dlen = ptr[idx++];

    enum ptypes t =  (enum ptypes) ptr[idx++];

    switch(t) {
        case(BOOL):
            break;
    }

}


/***********************************************************************
 *  Method: proto::mkGetBoolCmd
 *  Params: uint8_t sender, char *key, uint8_t *out
 * Returns: void
 * Effects: 
 ***********************************************************************/
void proto::mkGetBoolCmd(uint8_t sender, char *key, uint8_t *out) {
    uint8_t size=0;
    uint8_t idx=1;

    memset(out,0,MAX_PACKET);

    out[idx++] = GET;
    out[idx++] = sender;
    out[idx++] = strlen(key);

    memcpy(&out[idx], key, strlen(key));
    idx += strlen(key);
    out[idx] = BOOL;

    out[0] = idx;

}


/***********************************************************************
 *  Method: proto::mkSetBoolCmd
 *  Params: char *key, bool state, uint8_t *out
 * Returns: void
 * Effects: 
 ***********************************************************************/
void proto::mkSetBoolCmd(char *key, bool state, uint8_t *out) {
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


/***********************************************************************
 *  Method: proto::mkGetByteCmd
 *  Params: uint8_t sender, char *key, uint8_t *out
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
proto::mkGetByteCmd(uint8_t sender, char *key, uint8_t *out)
{
}


/***********************************************************************
 *  Method: proto::mkSetByteCmd
 *  Params: char *key, uint8_t state, uint8_t *out
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
proto::mkSetByteCmd(char *key, uint8_t state, uint8_t *out)
{
}


/***********************************************************************
 *  Method: proto::mkGetIntCmd
 *  Params: uint8_t sender, char *key, uint8_t *out
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
proto::mkGetIntCmd(uint8_t sender, char *key, uint8_t *out)
{
}


/***********************************************************************
 *  Method: proto::mkSetIntCmd
 *  Params: char *key, uint32_t state, uint8_t *out
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
proto::mkSetIntCmd(char *key, uint32_t state, uint8_t *out)
{
}


/***********************************************************************
 *  Method: proto::mkGetStringCmd
 *  Params: uint8_t sender, char *key, uint8_t *out
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
proto::mkGetStringCmd(uint8_t sender, char *key, uint8_t *out)
{
}


/***********************************************************************
 *  Method: proto::mkSetStringCmd
 *  Params: char *key, char *state, uint8_t *out
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
proto::mkSetStringCmd(char *key, char *state, uint8_t *out)
{
}

std::string proto::printCmd(enum cmd c) {

    std::string res;
    switch(c) {
        case NOP:
            res="NOP";
            break;
        case GET:
            res="GET" ;
            break;
        case SET:
            res="SET" ;
            break;
        case SUB:
            res="SUB" ;
            break;
        case UNSUB:
            res="UNSUB" ;
            break;
        case PING:
            res="PING" ;
            break;
        case EXIT:
            res="NEXT" ;
            break;
        default:
            res="WHAT?" ;
            break;
    }
    return res;
}
