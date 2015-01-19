
#include "BaseClient.h"

IOnvifClient* getOnvifClient()
{
    return BaseClient::Instance();
}

BaseClient::BaseClient():
    soap_ (soap_new())
{
}

#ifdef DEV_S
IOnvifDevMgmt* BaseClient::getDeviceClient() {
    return devClient_;
}
#endif
#ifdef DEVIO_S
IOnvifDevIO* BaseClient::getDeviceIOClient() {
    return devIOClient_;
}
#endif
#ifdef DISP_S
IOnvifDisplay* BaseClient::getDisplayClient() {
    return dispClient_;
}
#endif
#ifdef RECV_S
IOnvifReceiver* BaseClient::getReceiverClient() {
    return recvClient_;
}
#endif
#ifdef RECORD_S
IOnvifRecording* BaseClient::getRecordingClient() {
    return recordingClient_;
}
#endif
#ifdef MEDIA_S
IOnvifMedia* BaseClient::getMediaClient() {
	return mediaClient_;

}
#endif

BaseClient::~BaseClient()
{
    #ifdef DEV_S
    if (devClient_)
         delete devClient_;
    #endif
    #ifdef DEVIO_S
    if (devIOClient_)
        delete devIOClient_;
    #endif
    #ifdef DISP_S
    if (dispClient_)
        delete dispClient_;
    #endif
    #ifdef RECV_S
    if (recvClient_)
        delete recvClient_;
    #endif
    #ifdef RECORD_S
    if (recordingClient_)
        delete recordingClient_;
    #endif
    #ifdef EVNT_S
    if (notsConsumer_)
        delete notsConsumer_;
    #endif

     if(soap_) {
         soap_destroy(soap_);
         soap_end(soap_);
         soap_free(soap_);
     }
}

soap*
BaseClient::GetSoap() {
    return soap_;
}

int
BaseClient::Init(const char* pchEndpoint) {
    if( !soap_ ) {
        SIGRLOG (SIGRWARNING, "BaseClient::Init failed" );
        return -1;
    }

#ifdef DEV_S
    devClient_ = new DeviceClient (pchEndpoint, soap_);
#endif
#ifdef DEVIO_S
    devIOClient_ = new DeviceIOClient (pchEndpoint, soap_);
#endif
#ifdef DISP_S
    dispClient_ = new DisplayClient (pchEndpoint, soap_);
#endif
#ifdef RECV_S
    recvClient_ = new ReceiverClient (pchEndpoint, soap_);
#endif
#ifdef RECORD_S
    recordingClient_ = new RecordingClient (pchEndpoint, soap_);
#endif
#ifdef EVNT_S
    notsConsumer_ = new NotificationConsumer(pchEndpoint, soap_);
#endif
#ifdef MEDIA_S
    mediaClient_ = new MediaClient(pchEndpoint,soap_);
#endif
    if( !notsConsumer_ || !notsConsumer_->init() ) {
        SIGRLOG (SIGRWARNING, "BaseClient::Init failed to init NotificationConsumer" );
        return -1;
    }

    if( devClient_ )
        return 0;

    SIGRLOG (SIGRWARNING, "BaseClient::Init failed to create proxies" );
    return -1;
}

std::vector<DiscoveryMatch>
BaseClient::DiscoverDevices( OnvifDevice::Type type ) {
    return wsdd_.getMembers( DeviceInfoStorage::ResolveType(type) );
}

void
BaseClient::Subscribe() {
    notsConsumer_->subscribe();
}

bool
BaseClient::SetNotificationCatcher(notificationCatcherFunc func) {
    return notsConsumer_->setCatcher(func);
}
