#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "Protocol/proto.h"
#include "Dump/dump.h"

int main() {
    uint8_t idx=0;
    uint8_t outPacket[16] = { 0 };

    mkBoolCmd("LED", true, outPacket);
    dump(outPacket, 16);

    interpPacket(outPacket);
    dump(outPacket, 16);

    mkBoolCmd("FRED", 0xaa, outPacket);
    dump(outPacket, 16);

    exit(0);
}

