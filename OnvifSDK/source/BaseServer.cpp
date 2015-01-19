
#include "BaseServer.h"

static GeneratorInitializer generatorInitializer;

IOnvifService::~IOnvifService() {}
IOnvifHandler::~IOnvifHandler() {}

std::string
GenerateToken() {
    std::string str;
    for(unsigned int i = 0; i < TOKEN_LEN; ++i)
        str += alphanum[rand() % stringLength];
    return str;
}

IOnvifServer*
getOnvifServer() {
    return static_cast<IOnvifServer*>( new BaseServer() );
}

void
deleteOnvifServer(IOnvifServer* obj) {
    BaseServer::deleteObject(obj);
}

BaseServer::BaseServer():
    soap_(soap_new()) {
}

MedProfile
BaseServer::CreateMediaProfile( const std::string& name,
                                const std::string& token ) {
    return MedProfile(soap_, name , token);
}

MedVideoSource
BaseServer::CreateVideoSource( const std::string &token,
                               int w, int h, double frmRate ) {
    return MedVideoSource(soap_, token, w, h, frmRate);
}

MedVideoAnalyticsConfiguration
BaseServer::CreateVAConf( const std::string& name,
                          const std::string& token,
                          CellDetectionLayout::Fill fill ) {
    return MedVideoAnalyticsConfiguration(soap_, name, token, fill);
}

IOnvifService*
BaseServer::createService(OnvifService::Type type, IOnvifHandler * handler) {
    IOnvifService* service = NULL;
    switch(type) {
    case OnvifService::DEV:
        #ifdef DEV_S
            service = new DeviceServiceImpl( this, dynamic_cast<IOnvifDevMgmt*>(handler), soap_ );
        #endif
        break;
    case OnvifService::DEVIO:
        #ifdef DEVIO_S
            service = new DeviceIOServiceImpl( this, dynamic_cast<IOnvifDevIO*>(handler), soap_ );
        #endif
        break;
    case OnvifService::DISP:
        #ifdef DISP_S
            service = new DisplayServiceImpl( this, dynamic_cast<IOnvifDisplay*>(handler), soap_ );
        #endif
        break;
    case OnvifService::RECV:
        #ifdef RECV_S
            service = new ReceiverServiceImpl( this, dynamic_cast<IOnvifReceiver*>(handler), soap_ );
        #endif
        break;
    case OnvifService::REPLAY:
        #ifdef REPLAY_S
            service = new ReplayServiceImpl(this, soap_);
        #endif
        break;
    case OnvifService::RECORD:
        #ifdef RECORD_S
            service = new RecordingServiceImpl( this, dynamic_cast<IOnvifRecording*>(handler), soap_ );
        #endif
        break;
    case OnvifService::SEARCH:
        #ifdef SEARCH_S
            service = new SearchServiceImpl(this, soap_);
        #endif
        break;
    case OnvifService::MEDIA:
        #ifdef MEDIA_S
            service = new MediaServiceImpl(this, dynamic_cast<IOnvifMedia*>(handler), soap_);
        #endif
        break;
    case OnvifService::ANALY:
        #ifdef ANALY_S
            service = new AnalyticsServiceImpl(this, dynamic_cast<IOnvifAnalytics*>(handler), soap_);
        #endif
        break;
    case OnvifService::EVNT:
        #ifdef EVNT_S
            service = new NotificationProducer(this, soap_);
        #endif
        break;
    };

    return service;
}

int
BaseServer::Init( const OnvifHandlers& data ) {

    soap_init(soap_);
    soap_->bind_flags |= SO_REUSEADDR;

    for( int i = 0; i < ServicesAmount; ++i ) {
        if( data.h_[i] == NULL )
            continue;
        services_[static_cast<OnvifService::Type>(i)] =  createService( static_cast<OnvifService::Type>(i),
                                       data.h_[i] );
    }

    wsdd_ = new Discoverable();

    if( 0 != RunWsDiscovery() ) {
        SIGRLOG (SIGRWARNING, "BaseServer::Run RunWsDiscovery failed");
        return -1;
    }

    NotificationProducer* prod = dynamic_cast<NotificationProducer*>(services_[OnvifService::EVNT]);

    if( !prod || !prod->init() ) {
        SIGRLOG (SIGRWARNING, "BaseServer::Run NotificationProducer init failed");
        return -1;
    }

    created_ = soap_ && services_[OnvifService::DEV];
    return created_ ? 0 : -1;
}

BaseServer::~BaseServer() {
    SIGRLOG( SIGRDEBUG2, "BaseServer::~BaseServer enter");
    if( wsdd_ ) {
        delete wsdd_;
        wsdd_ = NULL;
    }

    for( std::map<OnvifService::Type, IOnvifService*>::iterator it = services_.begin();
         it != services_.end(); ++it ) {
        deleteService(it->second);
    }

    if( soap_ ) {
        soap_destroy(soap_);
        soap_end(soap_);
        soap_free(soap_);
    }

    SIGRLOG( SIGRDEBUG2, "BaseServer::~BaseServer exit");
}

int
BaseServer::Run() {
    if( !created_ ) {
        SIGRLOG(SIGRCRITICAL, "BaseServer::Run Services were not created");
        return -1;
    }

    int iRet = soap_bind(soap_, NULL, m_webservicePort, 100);

    if ( SOAP_INVALID_SOCKET == iRet ) {
        SIGRLOG(SIGRCRITICAL, "BaseServer::Run Binding on %d port failed", m_webservicePort);
        return -1;
    }

    while(1) {
        if( SOAP_INVALID_SOCKET == (iRet = soap_accept(soap_)) ) {
            SIGRLOG(SIGRCRITICAL, "BaseServer::Run accepting failed");
            return -1;
        }

        if( SOAP_OK != soap_begin_serve(soap_) ) {
            SIGRLOG(SIGRWARNING, "BaseServer::Run serve %d failed at %s", iRet, soap_->action);
            soap_destroy(soap_);
            continue;
        }

        for( std::map<OnvifService::Type, IOnvifService*>::iterator it = services_.begin();
             it != services_.end(); ++it ) {
            if(NULL == it->second)
                continue;

            if( SOAP_OK == (iRet = it->second->dispatch()) )
                break;
        }

        if( SOAP_OK != iRet )
            SIGRLOG(SIGRWARNING, "BaseServer::Run SOAP_Error= %d at %s", iRet, soap_->action);
        else
            SIGRLOG(SIGRDEBUG2, "BaseServer::Run SOAP_OK at %s", soap_->action);

        soap_destroy(soap_);
    }

    return 0;
}

int
BaseServer::SetDeviceInfo( OnvifDevice::Type type,
                               const std::string & manufacturer,
                               const std::string & model,
                               const std::string & firmwareVersion,
                               const std::string & serialNumber,
                               const std::string & hardwareId,
                               const std::string & scopes,
                               const std::string & interface,
                               int webservicePort ) {
    return DeviceInfoStorage::SetDeviceInfo( type, manufacturer, model,
                                             firmwareVersion, serialNumber, hardwareId,
                                             scopes, interface, webservicePort );
}

int
BaseServer::RunWsDiscovery() {
    if( !wsdd_ )
		return -1;
    return wsdd_->start(true, this);
}

std::string
BaseServer::GetIp() {
    return DeviceInfoStorage::getInterfaceIp("eth0");
}

void
BaseServer::SendNotification() {
    NotificationProducer* nots = dynamic_cast<NotificationProducer*>( services_[OnvifService::EVNT] );
    if( nots )
        nots->sendNotification();
}
