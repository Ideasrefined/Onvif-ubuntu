#ifndef NOTIFICATION_PRODUCER_H__
#define NOTIFICATION_PRODUCER_H__

#include <algorithm>

#include "pthread.h"

#include "onvifService.h"
#include "EventingClient.h"
#include "EventingServiceImpl.h"

// userEvents expects following messages
#define BUFFER_LEN 128
#define MSG_LEN 80
#define MSG_NAME "OnvifSDKMessage"

class NotificationProducer:
        public IOnvifService
{
public:
    NotificationProducer( BaseServer * pBaseServer, struct soap* pData );
    virtual ~NotificationProducer();

    bool init();
    int dispatch();
    void destroy();
    bool addConsumer( const std::string& consumerEndpoint );
    void stop();
    void sendNotification();
private:
    static void* notifyFunc(void* Producer);

    bool notify_;
    bool shutdownFlag_;
    int userEventsSocket_;
    std::vector<std::string> subscribers_;
    pthread_t thread_;
    pthread_mutex_t mutex_;
    soap* clientSoap_;
    EventingServiceImpl service_;
};

#endif // NOTIFICATION_PRODUCER_H__
