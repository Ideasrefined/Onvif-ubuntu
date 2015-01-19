#ifndef EVENTINGCLIENT_H
#define EVENTINGCLIENT_H

#include "sigrlog.h"
#include "OnvifSDK.h"
#include "WebPullPointSubscriptionBindingProxy.h"


class EventingClient
{
public:
    EventingClient(const char * pchAdress, soap * s);
    ~EventingClient();

    int Subscribe(EvntSubscribe & req, EvntSubscribeResponse & resp);
    int Notify(EvntNotify & req);
    soap * getProxySoap() { return m_proxy.soap; }
    void setEndpoint( const std::string& );

private:
    PullPointSubscriptionBindingProxy m_proxy;
};
#endif	// EVENTINGCLIENT_H
