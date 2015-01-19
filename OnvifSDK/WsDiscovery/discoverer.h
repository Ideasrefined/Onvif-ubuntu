#ifndef DISCOVERER__H
#define DISCOVERER__H

#include <tr1/memory>

#include "sigrlog.h"
#include "OnvifSDK.h"
#include "remotediscovery.hpp"

class Discoverer : public RemoteDiscovery
{
    public:
        Discoverer();
        virtual ~Discoverer();
        std::vector<DiscoveryMatch> getMembers( const std::string & onvifDeviceType );

        virtual void hello(const Hello_t & arg){}
        virtual void bye(const Bye_t & arg){}
        virtual ProbeMatches_t probe(const Probe_t & arg){}

    private:
        void search( const std::string & );
        bool collectNewMatches( const ProbeMatches_t );

        static const int probeAttemptsMaxAmount_ = 5;
        std::vector<DiscoveryMatch> members_;
        std::tr1::shared_ptr<Proxy_t> proxy_;
};

#endif // DISCOVERER__H
