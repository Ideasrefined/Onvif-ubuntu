#ifndef NOTIFICATION_CONSUMER_H__
#define NOTIFICATION_CONSUMER_H__
#include "pthread.h"

#include "EventingServiceImpl.h"
#include "EventingClient.h"
#include "DeviceInfoStorage.h"

class NotificationConsumer {
public:
    NotificationConsumer(const char * pchAdress, soap * s);
    ~NotificationConsumer();

    bool init();
    bool setCatcher(notificationCatcherFunc catcher);
    void notifyClient(const char*);
    void subscribe();
private:
    void stop();
    static void* notifyFunc(void* consumer);

    EventingClient eventingClient_;
    notificationCatcherFunc catcher_;
    bool shutdownFlag_;
    pthread_t thread_;
    pthread_mutex_t mutex_;
};

#endif //NOTIFICATION_CONSUMER_H__