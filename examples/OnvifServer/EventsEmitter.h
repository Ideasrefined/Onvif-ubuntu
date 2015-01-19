
#include "pthread.h"
#include "OnvifSDK.h"
class EventsEmitter {
public:
    EventsEmitter():shutdownFlag_(false) {}

    bool run();
    static void* emitFunc(void*);
    void stop();

private:
    bool shutdownFlag_;
    pthread_mutex_t mutex_;
    pthread_t thread_;
};
