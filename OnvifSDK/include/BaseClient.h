#ifndef BASE_CLIENT__H
#define BASE_CLIENT__H

#include "sigrlog.h"
#include "OnvifSDK.h"
#include "stringGenerator.h"

#include "DeviceClient.h"
#include "DeviceIOClient.h"
#include "DisplayClient.h"
#include "ReceiverClient.h"
#include "RecordingClient.h"
#include "EventingClient.h"

#include "discoverer.h"
#include "NotificationConsumer.h"


class BaseClient:
    public IOnvifClient {
public:
    static IOnvifClient* Instance() {
        static BaseClient theSingleInstance;
        return &theSingleInstance;
    }

    ~BaseClient();
    virtual int Init(const char* pchEndpoint);
    virtual std::vector<DiscoveryMatch> DiscoverDevices( OnvifDevice::Type type );
    virtual void Subscribe();
    virtual bool SetNotificationCatcher(notificationCatcherFunc func);
    virtual soap* GetSoap();

#ifdef DEV_S
    virtual IOnvifDevMgmt* getDeviceClient();
#endif
#ifdef DEVIO_S
    virtual IOnvifDevIO* getDeviceIOClient();
#endif
#ifdef DISP_S
    virtual IOnvifDisplay* getDisplayClient();
#endif
#ifdef RECV_S
    virtual IOnvifReceiver* getReceiverClient();
#endif
#ifdef RECORD_S
    virtual IOnvifRecording* getRecordingClient();
#endif
#ifdef MEDIA_S
    virtual IOnvifMedia* getMediaClient();
#endif
private:
    BaseClient();
    BaseClient(const BaseClient&);
    BaseClient& operator=(const BaseClient&);

    soap* soap_;

#ifdef DEV_S
    DeviceClient* devClient_;
#endif
#ifdef DEVIO_S
    DeviceIOClient* devIOClient_;
#endif
#ifdef DISP_S
    DisplayClient* dispClient_;
#endif
#ifdef RECV_S
    ReceiverClient* recvClient_;
#endif
#ifdef RECORD_S
    RecordingClient* recordingClient_;
#endif
#ifdef MEDIA_S
    MediaClient* mediaClient_;
#endif
#ifdef EVNT_S
    NotificationConsumer* notsConsumer_;
#endif

    Discoverer wsdd_;
};

#endif // BASE_CLIENT__H
