
#include "sigrlog.h"
#include "EventingServiceImpl.h"
#include "NotificationProducer.h"
#include "NotificationConsumer.h"
#include "BaseServer.h"

const std::string eventTopic = "<tns1:VideoAnalytics>"
    "<tns1:MotionDetection>"
        "<tns1:Motion wstop:topic=\"true\">"
            "<tt:MessageDescription>"
                "<tt:Source>"
                    "<tt:SimpleItemDescription Name=\"VideoAnalyticsConfigurationToken\" Type=\"tt:ReferenceToken\"/>"
                "</tt:Source>"
                "<tt:Data>"
                    "<tt:SimpleItemDescription Name=\"ObjectId\" Type=\"tt:ObjectRefType\"/>"
                "</tt:Data>"
            "</tt:MessageDescription>"
        "</tns1:Motion>"
    "</tns1:MotionDetection>"
"</tns1:VideoAnalytics>";

#warning TODO for while not supporting copy
PullPointSubscriptionBindingService* EventingServiceImpl::copy()
{
    return NULL;
}

int EventingServiceImpl::Subscribe(_wsnt__Subscribe *wsnt__Subscribe, _wsnt__SubscribeResponse *wsnt__SubscribeResponse)
{
    std::string consumer( wsnt__Subscribe->ConsumerReference.Address );

    wsnt__SubscribeResponse->SubscriptionReference.Address = (char *) soap_malloc(soap, sizeof(char) * INFO_LENGTH);
    strcpy( wsnt__SubscribeResponse->SubscriptionReference.Address,
            m_pBaseServer->m_endpoint.c_str() );

    time_t current = time(0);
    wsnt__SubscribeResponse->CurrentTime =  (time_t *) soap_malloc(soap, sizeof(time_t *) );
    memcpy(wsnt__SubscribeResponse->CurrentTime, &current, sizeof(time_t));
    wsnt__SubscribeResponse->TerminationTime = (time_t *) soap_malloc(soap, sizeof(time_t *) );
    current += 1000000;
    memcpy(wsnt__SubscribeResponse->TerminationTime, &current, sizeof(time_t));

    m_pNotsProducer->addConsumer( consumer );
    SIGRLOG( SIGRDEBUG2, "EventingServiceImpl::Subscribe %s\n", consumer.c_str() );
    return SOAP_OK;
}

int EventingServiceImpl::GetEventProperties( _tev__GetEventProperties *tev__GetEventProperties,
                                             _tev__GetEventPropertiesResponse *tev__GetEventPropertiesResponse)
{
    tev__GetEventPropertiesResponse->TopicNamespaceLocation.push_back( "http://www.onvif.org/onvif/ver10/topics/topicns.xml" );
    tev__GetEventPropertiesResponse->wsnt__FixedTopicSet = true;

    // tev__GetEventPropertiesResponse->wstop__TopicSet = (struct wstop__TopicSetType *)
    //         soap_malloc(soap, sizeof(struct wstop__TopicSetType));
    // soap_default__wstop__TopicSet(soap, tev__GetEventPropertiesResponse->wstop__TopicSet);

    tev__GetEventPropertiesResponse->wstop__TopicSet = soap_instantiate_wstop__TopicSetType(soap, eventTopic.length(), "", eventTopic.c_str(), NULL);
    tev__GetEventPropertiesResponse->wstop__TopicSet->documentation = soap_instantiate_wstop__Documentation(soap, eventTopic.length(), "", eventTopic.c_str(), NULL);

    // tev__GetEventPropertiesResponse->wstop__TopicSet->documentation->__mixed = (char*)soap_malloc(soap, sizeof(char) * LARGE_INFO_LENGTH);
    // strcpy( tev__GetEventPropertiesResponse->wstop__TopicSet->documentation->__mixed, m_sWSTopic.c_str() );
    // tev__GetEventPropertiesResponse->wstop__TopicSet->__item = (char*)soap_malloc(soap, sizeof(char) * LARGE_INFO_LENGTH);
    // strcpy( tev__GetEventPropertiesResponse->wstop__TopicSet->__item, m_sWSTopic.c_str() );
    tev__GetEventPropertiesResponse->wsnt__TopicExpressionDialect.push_back("http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet");
    tev__GetEventPropertiesResponse->wsnt__TopicExpressionDialect.push_back("http://docs.oasis-open.org/wsn/t-1/TopicExpression/Concrete");
    tev__GetEventPropertiesResponse->MessageContentFilterDialect.push_back("http://www.onvif.org/ver10/tev/messageContentFilter/ItemFilter");
    tev__GetEventPropertiesResponse->MessageContentSchemaLocation.push_back("http://www.onvif.org/ver10/schema/onvif.xsd");
    return SOAP_OK;
}

int EventingServiceImpl::Notify(_wsnt__Notify *wsnt__Notify) {
    if(m_pNotsConsumer && wsnt__Notify->NotificationMessage.size() > 0 )
        m_pNotsConsumer->notifyClient(wsnt__Notify->NotificationMessage[0]->Topic->__mixed);
    return SOAP_OK;
}

int EventingServiceImpl::Renew( _wsnt__Renew *wsnt__Renew,
                                _wsnt__RenewResponse *wsnt__RenewResponse) {
    SIGRLOG( SIGRDEBUG2, "EventingServiceImpl::Renew %s\n", wsnt__Renew->TerminationTime->c_str() );
    time_t current = time(0);
    current += 1000000;
    wsnt__RenewResponse->TerminationTime = current;
    return SOAP_OK;
}

