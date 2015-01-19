#ifndef WSDD_HPP
#define WSDD_HPP

#include <algorithm>
#include <tr1/memory>

#include <errno.h>

#include "OnvifSDK.h"
#include "helpers.h"
#include "sigrlog.h"
#include "remotediscovery.hpp"

class BaseServer;

class Discoverable : public RemoteDiscovery
{

public:
    Discoverable();
    virtual ~Discoverable();
    virtual int start( bool bIsDevice, BaseServer* serv = NULL );
    virtual int stop();

    virtual void hello(const Hello_t & arg);
    virtual void bye(const Bye_t & arg);
    virtual ProbeMatches_t probe(const Probe_t & arg);

private:
    static bool isMatched(const std::string & left, const std::string & right);
    void runDeviceService();

    static void * runServiceHelper(void * context) {
            ((Discoverable*)context)->runDeviceService();
    }


    bool stopped_;
    bool bExitRequest_;

    BaseServer * baseServ_;

    pthread_mutex_t mutex_;
    pthread_t serviceThread_;

    ProbeMatches_t probeMatches_;

    std::tr1::shared_ptr<Proxy_t> proxy_;
};

#endif // WSDD_HPP
