#include <string>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>

#define MAX_TASK 8

class mngmt{
    public:
        bool fred = false;
        static mngmt* Instance();

        void dump();
        mqd_t getTaskEntry(uint8_t taskId);

        bool initTask(uint8_t taskId);
        bool isTaskReady(uint8_t taskId);
        bool waitUntilReady(uint8_t taskId);
        bool waitUntilReady(uint8_t taskId, uint8_t ms);

    private:
        pthread_mutex_t queueLock;
        mqd_t queue[MAX_TASK];

        mngmt(); // Private so that it can  not be called
        mngmt(mngmt const&){ };             // copy constructor is private
        mngmt &operator=(mngmt const&){};  // assignment operator is private
        static mngmt *m_pInstance;

        mqd_t mkQueue(uint8_t taskId);
        void setTaskEntry(uint8_t taskId, mqd_t mq);
};
