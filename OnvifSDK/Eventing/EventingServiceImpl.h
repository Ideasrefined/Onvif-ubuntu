#ifndef WebEventingServiceImpl_H
#define WebEventingServiceImpl_H

#include "WebPullPointSubscriptionBindingService.h"

class BaseServer;
class NotificationProducer;
class NotificationConsumer;

    #define INFO_LENGTH 128
    #define LARGE_INFO_LENGTH 1024

    class EventingServiceImpl : public PullPointSubscriptionBindingService
    {
    private:
        BaseServer * m_pBaseServer;
        NotificationProducer * m_pNotsProducer;
        NotificationConsumer * m_pNotsConsumer;
    public:
        EventingServiceImpl( NotificationProducer *pNotsProd,
                             BaseServer * pBaseServer,
                             struct soap* pData ):
            m_pNotsProducer(pNotsProd),
            m_pBaseServer(pBaseServer),
            PullPointSubscriptionBindingService( pData ),
            m_pNotsConsumer(NULL)
        {
        }

        EventingServiceImpl( NotificationConsumer *pNotsCons,
                             BaseServer * pBaseServer,
                             struct soap* pData ):
            m_pNotsConsumer(pNotsCons),
            m_pBaseServer(pBaseServer),
            PullPointSubscriptionBindingService( pData ),
            m_pNotsProducer(NULL)
        {
        }

        virtual	PullPointSubscriptionBindingService *copy();

        virtual	int PullMessages(_tev__PullMessages *tev__PullMessages, _tev__PullMessagesResponse *tev__PullMessagesResponse) { return SOAP_ERR; }

        /// Web service operation 'Seek' (returns error code or SOAP_OK)
        virtual	int Seek(_tev__Seek *tev__Seek, _tev__SeekResponse *tev__SeekResponse) { return SOAP_ERR; }

        /// Web service operation 'SetSynchronizationPoint' (returns error code or SOAP_OK)
        virtual	int SetSynchronizationPoint(_tev__SetSynchronizationPoint *tev__SetSynchronizationPoint, _tev__SetSynchronizationPointResponse *tev__SetSynchronizationPointResponse) { return SOAP_ERR; }

        /// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
        virtual	int GetServiceCapabilities(_tev__GetServiceCapabilities *tev__GetServiceCapabilities, _tev__GetServiceCapabilitiesResponse *tev__GetServiceCapabilitiesResponse) { return SOAP_ERR; }

        /// Web service operation 'CreatePullPointSubscription' (returns error code or SOAP_OK)
        virtual	int CreatePullPointSubscription(_tev__CreatePullPointSubscription *tev__CreatePullPointSubscription, _tev__CreatePullPointSubscriptionResponse *tev__CreatePullPointSubscriptionResponse) { return SOAP_ERR; }

        /// Web service operation 'GetEventProperties' (returns error code or SOAP_OK)
        virtual	int GetEventProperties(_tev__GetEventProperties *tev__GetEventProperties, _tev__GetEventPropertiesResponse *tev__GetEventPropertiesResponse);

        /// Web service operation 'Renew' (returns error code or SOAP_OK)
        virtual	int Renew(_wsnt__Renew *wsnt__Renew, _wsnt__RenewResponse *wsnt__RenewResponse);

        /// Web service operation 'Unsubscribe' (returns error code or SOAP_OK)
        virtual	int Unsubscribe(_wsnt__Unsubscribe *wsnt__Unsubscribe, _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse) { return SOAP_ERR; }

        /// Web service operation 'Subscribe' (returns error code or SOAP_OK)
        virtual	int Subscribe(_wsnt__Subscribe *wsnt__Subscribe, _wsnt__SubscribeResponse *wsnt__SubscribeResponse);

        /// Web service operation 'GetCurrentMessage' (returns error code or SOAP_OK)
        virtual	int GetCurrentMessage(_wsnt__GetCurrentMessage *wsnt__GetCurrentMessage, _wsnt__GetCurrentMessageResponse *wsnt__GetCurrentMessageResponse) { return SOAP_ERR; }

        /// Web service one-way operation 'Notify' (return error code, SOAP_OK (no response), or send_Notify_empty_response())
        virtual	int Notify(_wsnt__Notify *wsnt__Notify);
        virtual	int send_Notify_empty_response(int httpcode) { return soap_send_empty_response(this->soap, httpcode); }

        /// Web service operation 'GetMessages' (returns error code or SOAP_OK)
        virtual	int GetMessages(_wsnt__GetMessages *wsnt__GetMessages, _wsnt__GetMessagesResponse *wsnt__GetMessagesResponse) { return SOAP_ERR; }

        /// Web service operation 'DestroyPullPoint' (returns error code or SOAP_OK)
        virtual	int DestroyPullPoint(_wsnt__DestroyPullPoint *wsnt__DestroyPullPoint, _wsnt__DestroyPullPointResponse *wsnt__DestroyPullPointResponse) { return SOAP_ERR; }

        /// Web service one-way operation 'Notify' (return error code, SOAP_OK (no response), or send_Notify_empty_response())
        virtual	int Notify_(_wsnt__Notify *wsnt__Notify) { return SOAP_ERR; }
        virtual	int send_Notify__empty_response(int httpcode) { return soap_send_empty_response(this->soap, httpcode); }

        /// Web service operation 'CreatePullPoint' (returns error code or SOAP_OK)
        virtual	int CreatePullPoint(_wsnt__CreatePullPoint *wsnt__CreatePullPoint, _wsnt__CreatePullPointResponse *wsnt__CreatePullPointResponse) { return SOAP_ERR; }

        /// Web service operation 'Renew' (returns error code or SOAP_OK)
        virtual	int Renew_(_wsnt__Renew *wsnt__Renew, _wsnt__RenewResponse *wsnt__RenewResponse) { return SOAP_ERR; }

        /// Web service operation 'Unsubscribe' (returns error code or SOAP_OK)
        virtual	int Unsubscribe_(_wsnt__Unsubscribe *wsnt__Unsubscribe, _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse) { return SOAP_ERR; }

        /// Web service operation 'PauseSubscription' (returns error code or SOAP_OK)
        virtual	int PauseSubscription(_wsnt__PauseSubscription *wsnt__PauseSubscription, _wsnt__PauseSubscriptionResponse *wsnt__PauseSubscriptionResponse) { return SOAP_ERR; }

        /// Web service operation 'ResumeSubscription' (returns error code or SOAP_OK)
        virtual	int ResumeSubscription(_wsnt__ResumeSubscription *wsnt__ResumeSubscription, _wsnt__ResumeSubscriptionResponse *wsnt__ResumeSubscriptionResponse) { return SOAP_ERR; }
    };

#endif //WebEventingServiceImpl_H
