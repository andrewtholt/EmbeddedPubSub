#include <stdint.h>
#include <stdbool.h>

#ifdef LINUX
#include <mqueue.h>

extern pthread_mutex_t queueLock;
#endif

#define MAX_TASK 8
bool waitUntilReady(uint8_t taskId);
void setTaskEntry(uint8_t taskId, mqd_t mq) ;
mqd_t getTaskEntry(uint8_t taskId);
bool isTaskReady(uint8_t taskId) ;
mqd_t mkQueue(uint8_t taskId);

// 
// Get message int *msg, return length.  -ve indicates eror.
//
void msgInit();
int getMsg( uint8_t task, void *msg, uint16_t len );
int getMsgWait( uint8_t task, void *msg, uint16_t len, uint16_t ms );
// 
// Put message int *msg, return length.  -ve indicates eror.
//
int putMsg( uint8_t task, void *msg, uint16_t len );
int putMsgWait( uint8_t task, void *msg, uint16_t len, uint16_t ms );

int queueLen( uint8_t task);

bool queueFull(uint8_t task);
bool queueEmpty(uint8_t task);


