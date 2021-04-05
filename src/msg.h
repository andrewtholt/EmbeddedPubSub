#include <stdint.h>
#include <atdbool.h>

void *queue[ MAX_TASK]
// 
// Get message int *msg, return length.  -ve indicates eror.
//
int getMsg( uint8_t task, void *msg. uint16_t len );
int getMsgWait( uint8_t task, void *msg. uint16_t len, uint16_t ms );
// 
// Put message int *msg, return length.  -ve indicates eror.
//
int putMsg( uint8_t task, void *msg. uint16_t len );
int putMsgWait( uint8_t task, void *msg. uint16_t len. uint16_t ms );

int queueLen( uint8_t task);

bool queueFull(uint8_t task);
bool queueEmpty(uint8_t task);


