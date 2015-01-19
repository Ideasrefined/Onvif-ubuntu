
#include "EventingClient.h"

EventingClient::EventingClient(const char * pchAdress, soap * s) :
    m_proxy(s)
{
    m_proxy.soap_endpoint = pchAdress;
}

EventingClient::~EventingClient()
{
}

int EventingClient::Subscribe(EvntSubscribe & req, EvntSubscribeResponse & resp)
{
    int nRes = m_proxy.Subscribe(req.d, resp.d);
    CHECKRETURN(nRes, "EventingClient::Subscribe");
}

int EventingClient::Notify(EvntNotify & req)
{
    int nRes = m_proxy.send_Notify( m_proxy.soap_endpoint,
                                    NULL,
                                    req.d ); //m_proxy.Notify(req.d);
    CHECKRETURN(nRes, "EventingClient::Notify");
}

void EventingClient::setEndpoint( const std::string& endpoint ) {
    m_proxy.soap_endpoint = endpoint.c_str();
}

