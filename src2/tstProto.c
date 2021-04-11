#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "proto.h"
#include "dump.h"
#include "kvs.h"

#include <mqueue.h>

#define MAX_THREADS 10

KVSstore *store = NULL;

int main() {

    struct mq_attr tasks[MAX_THREADS] = { 0 };

    store = kvs_create(strcmp);

    uint8_t idx=0;
    uint8_t outPacket[MAX_PACKET] = { 0 };
    uint8_t  inPacket[MAX_PACKET] = { 0 };

    printf("Set Bool\n");
    mkSetBoolCmd("LED", true, outPacket);
    dump(outPacket, 32);

    interpPacket(outPacket, inPacket);
    dump(outPacket, 32);

    printf("Get Bool\n");
    mkGetBoolCmd(1,"LED", outPacket);
    dump(outPacket, 32);

    printf("===================================\n");

    printf("Set Byte\n");
    mkSetByteCmd("FRED", 0xab, outPacket);
    dump(outPacket, 32);

    interpPacket(outPacket, inPacket);
    dump(outPacket, 32);

    printf("Get Byte\n");
    mkGetByteCmd(2,"FRED", outPacket);
    dump(outPacket, 32);
    printf("===================================\n");

    printf("Set Int\n");
    mkSetIntCmd("BILLY", 0x123456, outPacket);
    dump(outPacket, 32);

    interpPacket(outPacket, inPacket);
    dump(outPacket, 32);

    printf("Get Int\n");
    mkGetIntCmd(3,"BILLY", outPacket);
    dump(outPacket, 32);
    printf("===================================\n");

    printf("Set String\n");
    mkSetStringCmd("WILLIAM", "Some data", outPacket);
    dump(outPacket, 32);

    interpPacket(outPacket, inPacket);
    dump(outPacket, 32);

    printf("Get String\n");
    mkGetStringCmd(4, "WILLIAM", outPacket);
    dump(outPacket, 32);
    printf("===================================\n");

    exit(0);
}

