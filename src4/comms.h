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

