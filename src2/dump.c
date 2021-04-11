#include <stdint.h>
#include <stdio.h>
#include "dump.h"

#ifdef __cplusplus
extern "C" {
#endif

void dumpHexLine(uint8_t *m) {
    for( int i=0; i< BYTES_PER_LINE ;i++) {
        printf("%02x ",m[i]);
    }
}

void dumpAsciiLine(uint8_t *m) {
    for( int i=0; i< BYTES_PER_LINE ;i++) {
        if ( m[i] < 32 ) { 
            printf(".");
        } else {
            printf("%c",m[i]);
        }
    }
}

void dumpLine(uint8_t *m) {
    dumpHexLine(m);
    printf(":");
    dumpAsciiLine(m);
    printf("\n");
}

void dump(uint8_t *mem, int count) {
    uint8_t *m = mem;
    for (int i=0; i < count; i+=8) {
        dumpLine(m);
        m += BYTES_PER_LINE;
    }
}

#ifdef __cplusplus
}
#endif

