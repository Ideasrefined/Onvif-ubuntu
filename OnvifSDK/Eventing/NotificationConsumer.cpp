
#include "sigrlog.h"
#include "NotificationConsumer.h"


NotificationConsumer::NotificationConsumer(const char * endpoint, soap * soap):
    eventingClient_(endpoint, soap),
    shutdownFlag_(false)
{
    pthread_mutex_init(&mutex_, NULL);
}

NotificationConsumer::~NotificationConsumer() {
    stop();
}

bool
NotificationConsumer::init() {

    if( pthread_create(&thread_, NULL, &NotificationConsumer::notifyFunc, this ) != 0) {
        SIGRLOG (SIGRWARNING, "NotificationConsumer::init failed to run notifyFunc" );
        return false;
    }

    return true;
}

void
NotificationConsumer::stop() {
    pthread_mutex_lock( &mutex_ );
    shutdownFlag_ = true;
    pthread_mutex_unlock( &mutex_ );
    pthread_join(thread_, NULL);
}

void
NotificationConsumer::subscribe() {


    EvntSubscribe req(eventingClient_.getProxySoap());
    std::stringstream consumerEndpoint;
    consumerEndpoint << "http://" <<
            DeviceInfoStorage::getInterfaceIp("eth0") <<
            ":8080";
    req.setAddress( consumerEndpoint.str() );
    EvntSubscribeResponse resp(eventingClient_.getProxySoap());
    eventingClient_.Subscribe(req, resp);
}

bool
NotificationConsumer::setCatcher( notificationCatcherFunc catcher ) {
    if(!catcher) {
        SIGRLOG (SIGRWARNING, "NotificationConsumer::setCatcher provided arg is NULL" );
        return false;
    }
    catcher_ = catcher;
    return true;
}

void
NotificationConsumer::notifyClient(const char* msgText) {
    if(catcher_)
        catcher_(msgText);
}

void* NotificationConsumer::notifyFunc(void* ptr) {
    NotificationConsumer* consumer = static_cast<NotificationConsumer*>(ptr);
    int iRet;
    EventingServiceImpl* serv = new EventingServiceImpl(consumer, NULL, soap_new());

    if ( (iRet = soap_bind(serv->soap, NULL, 8080, 100) == SOAP_INVALID_SOCKET ) ) {
        SIGRLOG(SIGRCRITICAL, "NotificationConsumer::notifyFunc Binding on %d port failed", 8080);
        return NULL;
    }

    while(1) {
        bool bBreak = false;
        pthread_mutex_lock( &consumer->mutex_ );
        if(consumer->shutdownFlag_) bBreak = true;
        pthread_mutex_unlock( &consumer->mutex_ );
        if(bBreak)
            break;

        if( (iRet = soap_accept(serv->soap)) == SOAP_INVALID_SOCKET) {
            SIGRLOG(SIGRCRITICAL, "NotificationConsumer::notifyFunc accepting failed");
            return NULL;
        }

        if ( (soap_begin_serve(serv->soap)) != SOAP_OK) {
            SIGRLOG(SIGRWARNING, "NotificationConsumer::notifyFunc serve %d failed", iRet);
            continue;
        }

        if( serv )
            iRet = serv->dispatch();
        continue;
    }

    serv->destroy();
}
