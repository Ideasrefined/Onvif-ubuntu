
#include "NotificationProducer.h"
#include "Web.nsmap"

NotificationProducer::NotificationProducer( BaseServer * pBaseServer, struct soap* pData ) :
    service_(this, pBaseServer, pData),
    clientSoap_(soap_new()),
    shutdownFlag_(true),
    notify_(false)
{
    pthread_mutex_init(&mutex_, NULL);
}

NotificationProducer::~NotificationProducer() {
    SIGRLOG (SIGRDEBUG2, "NotificationProducer::~NotificationProducer");
    stop();
    pthread_mutex_destroy(&mutex_);

    if( !clientSoap_ )
        return;
    soap_destroy( clientSoap_ );
    soap_end( clientSoap_ );
    soap_free( clientSoap_ );
}



int NotificationProducer::dispatch() {
    return service_.dispatch();
}

void NotificationProducer::destroy() {
    service_.destroy();
}

bool NotificationProducer::init() {
    shutdownFlag_ = false;
    if( pthread_create(&thread_, NULL, &NotificationProducer::notifyFunc, this ) != 0) {
        SIGRLOG (SIGRWARNING, "NotificationProducer::Init failed to create serv" );
        return false;
    }
    return true;
}

bool NotificationProducer::addConsumer( const std::string& consumerEndpoint ) {
    bool bRet = false;
    pthread_mutex_lock( &mutex_ );
    if( std::find( subscribers_.begin(), subscribers_.end(), consumerEndpoint ) == subscribers_.end() ) {
        subscribers_.push_back( consumerEndpoint );
        bRet = true;
    }

    pthread_mutex_unlock( &mutex_ );
    return bRet;
}

void NotificationProducer::stop() {
    SIGRLOG( SIGRDEBUG2, "NotificationProducer::stop enter" );
    bool wasStoppedAlready = false;
    pthread_mutex_lock( &mutex_ );
    if( shutdownFlag_ )
        wasStoppedAlready = true;
    shutdownFlag_ = true;
    pthread_mutex_unlock( &mutex_ );

    if( wasStoppedAlready ) {
        SIGRLOG( SIGRDEBUG2, "NotificationProducer::stop was stopped already. exiting" );
        return;
    }

    pthread_join(thread_, NULL);
    SIGRLOG( SIGRDEBUG2, "NotificationProducer::stop thread exited" );
}

void
NotificationProducer::sendNotification() {
    SIGRLOG( SIGRDEBUG2, "NotificationProducer::sendNotification enter" );
    pthread_mutex_lock( &mutex_ );
    if( !shutdownFlag_ && !notify_ ) {
        notify_ = true;
        SIGRLOG( SIGRDEBUG2, "NotificationProducer::sendNotification notify_ true and subs size %d", subscribers_.size() );
    }
    pthread_mutex_unlock( &mutex_ );
}

void*
NotificationProducer::notifyFunc(void* ptr) {
    NotificationProducer* prod = static_cast<NotificationProducer*>(ptr);
    if( !prod ) {
        SIGRLOG (SIGRWARNING, "NotificationProducer::notifyFunc Erroneous arg");
        return NULL;
    }

    EventingClient client("", prod->clientSoap_);
    pthread_mutex_t* pMutex = &prod->mutex_;
    soap_header(client.getProxySoap());
    client.getProxySoap()->header->wsa5__Action = (char *) soap_malloc( client.getProxySoap(), 1024 );
    strcpy(client.getProxySoap()->header->wsa5__Action, "http://docs.oasis-open.org/wsn/bw-2/NotificationConsumer/Notify");
    client.getProxySoap()->header->wsa__MessageID = NULL;
    client.getProxySoap()->header->wsa__To = NULL;
    client.getProxySoap()->header->wsa__Action = NULL;
    soap_set_namespaces(client.getProxySoap(), Web_namespaces);

    while(1) {
        usleep( 10000 );

        bool bCont = true;
        bool bBreak = false;

        pthread_mutex_lock( pMutex );
            if( prod->shutdownFlag_ ) {
                SIGRLOG (SIGRDEBUG2, "NotificationProducer::notifyFunc break");
                bBreak = true;
            } else if( prod->subscribers_.size() > 0 && prod->notify_ ) {
                bCont = false;
                prod->notify_ = false;
            }
        pthread_mutex_unlock( pMutex );

        if(bBreak) break;
        if(bCont) continue;

        EvntNotify req(client.getProxySoap());
        wsnt__NotificationMessageHolderType *holder =
                soap_instantiate_wsnt__NotificationMessageHolderType(client.getProxySoap(), -1, "", "", NULL);
        wsnt__TopicExpressionType * topic =
                soap_instantiate_wsnt__TopicExpressionType( client.getProxySoap(), -1, "", "", NULL );
        topic->Dialect = "http://docs.oasis-open.org/wsn/t-1/TopicExpression/Concrete";
        holder->Topic = topic;
        holder->SubscriptionReference = NULL;
        holder->ProducerReference = NULL;
        req.d->NotificationMessage.push_back(holder);

        pthread_mutex_lock( pMutex );
        std::vector<std::string> localSubs =  prod->subscribers_;
        pthread_mutex_unlock( pMutex );

        for( std::vector<std::string>::const_iterator it = localSubs.begin();
             it != localSubs.end(); ++it ) {
            client.setEndpoint( *it );
		    SIGRLOG (SIGRDEBUG2, "NotificationProducer::notifyFunc send it %s", it->c_str());
            client.Notify(req);
        }
    }

    return NULL;
}
