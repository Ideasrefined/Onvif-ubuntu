
#include "discoverable.h"
#include "BaseServer.h"

Discoverable::Discoverable() :
    stopped_(false),
  bExitRequest_(false)
{
    pthread_mutex_init( &mutex_, NULL );
}

Discoverable::~Discoverable()
{
    SIGRLOG(SIGRDEBUG2, "Discoverable::~Discoverable");
    stop();
    pthread_mutex_destroy(&mutex_);
}

int Discoverable::start(bool bIsDevice, BaseServer *serv )
{
    if( serv == NULL )
        return -1;
    baseServ_ = serv;
    int iRet = pthread_create(&serviceThread_, NULL, &Discoverable::runServiceHelper, this);
    return iRet;
}

int Discoverable::stop()
{
    pthread_mutex_lock(&mutex_);
    bExitRequest_ = true;
    pthread_mutex_unlock(&mutex_);
    pthread_join(serviceThread_, NULL);
    return 0;
}

void Discoverable::hello(const Hello_t & arg)
{
    SIGRLOG( SIGRDEBUG3,  "hello ( %s, %s, %s)",
              (arg.xaddrs != NULL ? arg.xaddrs->c_str() : ""),
              (arg.types != NULL ?  arg.types->c_str() : ""),
              (arg.scopes != NULL ? arg.scopes->item.c_str() : ""));
// TODO: implement
// currently not used, since wsdd client get devices only by probe request
//    if(arg.xaddrs != NULL && !arg.xaddrs->empty())
//    {
//        if (std::find(members_.begin(), members_.end(), *arg.xaddrs) == members_.end())
//            members_.push_back(arg.xaddrs->c_str());
//    }
}

void Discoverable::bye(const Bye_t & arg)
{
    SIGRLOG( SIGRDEBUG3,  "bye ( %s, %s, %s)",
              (arg.xaddrs != NULL ? arg.xaddrs->c_str() : ""),
              (arg.types != NULL ? arg.types->c_str() : ""),
              (arg.scopes != NULL ? arg.scopes->item.c_str() : ""));
}

Discoverable::ProbeMatches_t  Discoverable::probe(const Probe_t & arg)
{
    SIGRLOG( SIGRDEBUG3,  "probe ( %s, %s)",
             (arg.types != NULL ? arg.types->c_str() : ""),
             (arg.scopes != NULL ? arg.scopes->item.c_str() : ""));

    bool matched = true;
    if (arg.types != NULL && probeMatches_.back().types != NULL) {
        std::string types = *arg.types;
        while (true) {
            std::size_t pos1 = types.rfind(':');
            if (pos1 == std::string::npos)
                break;

            std::size_t pos2 = types.rfind(' ', pos1);
            if (pos2 == std::string::npos)
                pos2 = 0;
            else
                ++pos2;

            types.erase(pos2, pos1 - pos2 + 1);
        }

        matched = isMatched(types, *probeMatches_.back().types);
    }

    if (matched && arg.scopes != NULL && !arg.scopes->item.empty() && probeMatches_.back().scopes != NULL)
        matched = isMatched(arg.scopes->item, probeMatches_.back().scopes->item);

    return matched ? probeMatches_ : Discoverable::ProbeMatches_t();
}

bool Discoverable::isMatched(const std::string & left, const std::string & right)
{
    std::vector<std::string> l;
    std::vector<std::string> r;

    l = split(left, ' ');
    r = split(right, ' ');

    for( std::vector<std::string>::iterator it = l.begin(); it != l.end(); ++it ) {
        std::vector<std::string> item = split(*it, ':');
        *it = item.back();
    }

    for( std::vector<std::string>::iterator it = r.begin(); it != r.end(); ++it ) {
        std::vector<std::string> item = split(*it, ':');
        *it = item.back();
    }

    return std::find_first_of(l.begin(), l.end(), r.begin(), r.end()) != l.end();
}

void Discoverable::runDeviceService() {
    try {
        std::tr1::shared_ptr<Service_t> service(RemoteDiscovery::service());
        std::string address = "urn:";
        address += randUuid( service->getSoap() );
        std::string xaddrs = baseServ_->m_endpoint;

        Scopes_t scopes = Scopes_t();
        scopes.item = vector_join(baseServ_->m_scopes, " ");
        EndpointReference_t endpoint = EndpointReference_t();
        endpoint.address = &address;

        probeMatches_.resize(1);
        probeMatches_.back().endpoint = &endpoint;
        probeMatches_.back().scopes = &scopes;
        probeMatches_.back().types = &baseServ_->m_type;
        probeMatches_.back().xaddrs = &xaddrs;
        probeMatches_.back().version = 1;

        proxy_.reset(RemoteDiscovery::proxy());
        proxy_->hello(probeMatches_.back());

        SIGRLOG( SIGRDEBUG2,  "[runDeviceService] Starting the service loop" );
        if( service->bind(this) == -1 )
            throw UnixException( errno );

        while (true) {
            pthread_mutex_lock(&mutex_);
            if(bExitRequest_)
            {
                service->destroy();
                bExitRequest_ = false;
                break;
            }
            pthread_mutex_unlock(&mutex_);

            if (service->accept() == -1) {
              SIGRLOG( SIGRWARNING,  "Accept failed" );
              continue;
            }

            if( service->serve() != 0 )
                continue;

            service->destroy();
        }

        service.reset();
        proxy_.reset(RemoteDiscovery::proxy());
        proxy_->bye(probeMatches_.back());

    } catch (std::exception & ex) {
        SIGRLOG( SIGRWARNING, "[runDeviceService] Caught an exception: %s", ex.what() );
    }

    SIGRLOG( SIGRDEBUG0,  "[runDeviceService] The service loop stopped" );
}
