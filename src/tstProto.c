#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "proto.h"
#include "dump.h"
#include "kvs.h"

KVSstore *store = NULL;

int main() {

    store = kvs_create(strcmp);

    uint8_t idx=0;
    uint8_t outPacket[MAX_PACKET] = { 0 };
    uint8_t  inPacket[MAX_PACKET] = { 0 };

    printf("Bool\n");
    mkSetBoolCmd("LED", true, outPacket);
    dump(outPacket, 32);

    interpPacket(outPacket, inPacket);
    dump(outPacket, 32);

    printf("Get Bool\n");
    mkGetBoolCmd("LED", outPacket);
    dump(outPacket, 32);

    printf("===================================\n");

    printf("Byte\n");
    mkSetByteCmd("FRED", 0xab, outPacket);
    dump(outPacket, 32);

    interpPacket(outPacket, inPacket);
    dump(outPacket, 32);
    printf("===================================\n");

    printf("Int\n");
    mkSetIntCmd("BILLY", 0x123456, outPacket);
    dump(outPacket, 32);

    interpPacket(outPacket, inPacket);
    dump(outPacket, 32);
    printf("===================================\n");

    printf("String\n");
    mkSetStringCmd("WILLIAM", "Some data", outPacket);
    dump(outPacket, 32);

    interpPacket(outPacket, inPacket);
    dump(outPacket, 32);
    printf("===================================\n");

    exit(0);
}

