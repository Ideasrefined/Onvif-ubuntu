#ifndef BASESERVER_H
#define BASESERVER_H

#include <map>

#include "defs.h"

#include "sigrlog.h"
#include "onvifService.h"
#include "OnvifSDK.h"
#include "stringGenerator.h"
#include "DeviceInfoStorage.h"
#include "discoverable.h"

#ifdef DEV_S
#include "DeviceServiceImpl.h"
#endif
#ifdef DEVIO_S
#include "DeviceIOServiceImpl.h"
#endif
#ifdef DISP_S
#include "DisplayServiceImpl.h"
#endif
#ifdef RECV_S
#include "ReceiverServiceImpl.h"
#endif
#ifdef REPLAY_S
#include "ReplayServiceImpl.h"
#endif
#ifdef MEDIA_S
#include "MediaServiceImpl.h"
#endif
#ifdef RECORD_S
#include "RecordingServiceImpl.h"
#endif
#ifdef SEARCH_S
#include "SearchServiceImpl.h"
#endif
#ifdef ANALY_S
#include "AnalyticsServiceImpl.h"
#endif
#ifdef EVNT_S
#include "NotificationProducer.h"
#endif

class BaseServer : public DeviceInfoStorage,
                   public IOnvifServer
{
public:
    BaseServer();
    static void deleteObject(IOnvifServer* obj) {
        BaseServer* servObj = static_cast<BaseServer*> (obj);
        if(servObj)
            delete servObj;
    }

	virtual ~BaseServer();
    virtual int Init( const OnvifHandlers& );
	virtual int Run();
    virtual MedProfile CreateMediaProfile(const std::string &name, const std::string &token);
    virtual MedVideoSource CreateVideoSource(const std::string &token, int w, int h, double frmRate);
    virtual std::string GetIp();
    virtual MedVideoAnalyticsConfiguration CreateVAConf( const std::string& name,
                                                         const std::string& token,
                                                         CellDetectionLayout::Fill fill );
    virtual void SendNotification();

    int SetDeviceInfo( OnvifDevice::Type type,
                       const std::string & manufacturer,
                       const std::string & model,
                       const std::string & firmwareVersion,
                       const std::string & serialNumber,
                       const std::string & hardwareId,
                       const std::string & scopes,
                       const std::string & interface,
                       int webservicePort );

private:
    BaseServer(const BaseServer&);
    BaseServer& operator=(const BaseServer&);
    IOnvifService* createService( OnvifService::Type type,
                                  IOnvifHandler * handler );
    virtual int RunWsDiscovery();

    template<typename T> void
    deleteService(T* service) {
        if(!service)
            return;
        service->destroy();
        delete service;
        service = NULL;
    }

    bool created_;
    struct soap * soap_;
    std::map<OnvifService::Type, IOnvifService*> services_;
    Discoverable * wsdd_;
};

#endif	// BASESERVER_H
