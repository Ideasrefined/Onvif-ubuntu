#include "wsa.hpp"
//#include <wsaapi.h>

namespace  {
/** Anonymous Reply/To endpoint address */
static const std::string ANONYMOUS_URI = "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous";
/** Specifies no Reply endpoint address (no reply) */
static const std::string NONE_URI = "addressing/none not supported";
static const std::string FAULT_ACTION = "http://schemas.xmlsoap.org/ws/2004/08/addressing/fault";

int wsaError(soap * s, wsa__FaultSubcodeValues fault)
{
    const char * code = soap_wsa__FaultSubcodeValues2s(s, fault);
    switch (fault) {
    case wsa__FaultSubcodeValues__wsa__InvalidMessageInformationHeader:
        return soap_sender_fault_subcode(s, code, "A message information header is not valid and the message cannot be processed. The validity failure can be either structural or semantic, e.g. a [destination] that is not a URI or a [relationship] to a [message id] that was never issued.", "Invalid header");
    case wsa__FaultSubcodeValues__wsa__MessageInformationHeaderRequired:
        return soap_sender_fault_subcode(s, code, "A required message information header, To, MessageID, or Action, is not present.", "Missing Header QName");
    case wsa__FaultSubcodeValues__wsa__DestinationUnreachable:
        return soap_sender_fault_subcode(s, code, "No route can be determined to reach the destination role defined by the WS-Addressing To.", NULL);
    case wsa__FaultSubcodeValues__wsa__ActionNotSupported:
        return soap_sender_fault_subcode(s, code, "The [action] cannot be processed at the receiver.", s->action);
    case wsa__FaultSubcodeValues__wsa__EndpointUnavailable:
        return soap_sender_fault(s, code, "The endpoint is unable to process the message at this time.");
    default:
        break;
    }

    return SOAP_FAULT;
}

};


Wsa::Wsa(struct soap * soap) :
    soap_(soap)
{
    sequence_.soap_default(soap);
}

Wsa::~Wsa()
{
    soap_destroy(soap_);
    soap_end(soap_);
}

uint & Wsa::instanceId()
{
    static uint rv = time(NULL);
    return rv;
}

std::string & Wsa::sequenceId()
{
    static std::string  rv;
    return rv;
}

uint & Wsa::messageNumber()
{
    static uint rv = 0;
    ++rv;
    return rv;
}

int Wsa::allocHeader()
{
    soap_header(soap_);
    if (soap_->header != NULL)
        return SOAP_OK;

    return (soap_->error = SOAP_EOM);
}

int Wsa::check() const
{
    if (soap_->header == NULL || soap_->header->SOAP_WSA(Action) == NULL || strcmp(soap_->header->SOAP_WSA(Action), "") == 0 )
        return wsaError(soap_, wsa__FaultSubcodeValues__wsa__MessageInformationHeaderRequired);

    return SOAP_OK;
}

int Wsa::addFrom(const std::string & from)
{
    if (soap_->header == NULL)
        return SOAP_ERR;

    soap_->header->SOAP_WSA(From) = new wsa__EndpointReferenceType;
    soap_->header->SOAP_WSA(From)->soap_default(soap_);
    soap_->header->SOAP_WSA(From)->Address->__item = from;
    return SOAP_OK;
}

int Wsa::addNoReply()
{
    return addReplyTo(NONE_URI);
}

int Wsa::addReplyTo(const std::string & replyTo)
{
    if (soap_->header == NULL)
        return SOAP_ERR;

    if (!replyTo.empty()) {
        soap_->header->SOAP_WSA(ReplyTo) = new wsa__EndpointReferenceType;
        soap_->header->SOAP_WSA(ReplyTo)->soap_default(soap_);
        soap_->header->SOAP_WSA(ReplyTo)->Address->__item = replyTo;
    }

    return SOAP_OK;
}

int Wsa::addFaultTo(const std::string & faultTo)
{
    if (soap_->header == NULL)
        return SOAP_ERR;

    if (!faultTo.empty()) {
        soap_->header->SOAP_WSA(FaultTo) = new wsa__EndpointReferenceType;
        soap_->header->SOAP_WSA(FaultTo)->soap_default(soap_);
        soap_->header->SOAP_WSA(FaultTo)->Address->__item = faultTo;
    }

    return SOAP_OK;
}

int Wsa::addRelatesTo(const std::string & relatesTo)
{
    if (soap_->header == NULL)
        return SOAP_ERR;

    if (!relatesTo.empty()) {
        soap_->header->SOAP_WSA(RelatesTo) = new wsa__Relationship;
        soap_->header->SOAP_WSA(RelatesTo)->soap_default(soap_);
        soap_->header->SOAP_WSA(RelatesTo)->__item = relatesTo;
    }

    return SOAP_OK;
}

int Wsa::addAppSequence(std::string * id)
{
    sequence_.SequenceId = id;
    sequence_.InstanceId = instanceId();
    sequence_.MessageNumber = messageNumber();
    soap_->header->wsd__AppSequence = &sequence_;
}


int Wsa::reply(const std::string & id, const std::string & action)
{
//    struct SOAP_ENV__Header * oldheader = soap_->header;
//    soap_->header = NULL;

//    /* if endpoint address for reply is 'none' return immediately */
//    if (oldheader != NULL && oldheader->SOAP_WSA(ReplyTo) && oldheader->SOAP_WSA(ReplyTo)->Address
//            && !strcmp(oldheader->SOAP_WSA(ReplyTo)->Address, NONE_URI))
//    {
//        return soap_send_empty_response(soap_, SOAP_OK);
//    }

//    /* allocate a new header */
//    if (alloHeader() != 0)
//        return soap_->error;

//    struct SOAP_ENV__Header * newheader = soap_->header;
//    if (oldheader != NULL)
//        *newheader = *oldheader;

//    newheader->SOAP_WSA(MessageID) = id;
//    newheader->SOAP_WSA(Action) = action;
//    newheader->SOAP_WSA(RelatesTo) = NULL;
//    newheader->SOAP_WSA(From) = NULL;
//    newheader->SOAP_WSA(To) = NULL;
//    newheader->SOAP_WSA(ReplyTo) = NULL;
//    newheader->SOAP_WSA(FaultTo) = NULL;


//    if (oldheader && oldheader->SOAP_WSA(MessageID)) {
//        newheader->SOAP_WSA(RelatesTo) = (SOAP_WSA_(,RelatesTo)*)soap_malloc(soap, sizeof(SOAP_WSA_(,RelatesTo)));
//        SOAP_WSA_(soap_default_,RelatesTo)(soap, newheader->SOAP_WSA(RelatesTo));
//        newheader->SOAP_WSA(RelatesTo)->__item = oldheader->SOAP_WSA(MessageID);
//    }

//    if (oldheader && oldheader->SOAP_WSA(ReplyTo) && oldheader->SOAP_WSA(ReplyTo)->Address && strcmp(oldheader->SOAP_WSA(ReplyTo)->Address, soap_wsa_anonymousURI)) {
//        newheader->SOAP_WSA(To) = oldheader->SOAP_WSA(ReplyTo)->Address;
//        /* (re)connect to ReplyTo endpoint if From != ReplyTo */
//        if (!oldheader->SOAP_WSA(From) || !oldheader->SOAP_WSA(From)->Address || strcmp(oldheader->SOAP_WSA(From)->Address, oldheader->SOAP_WSA(ReplyTo)->Address)) {
//            struct soap *reply_soap = soap_copy(soap);
//            if (reply_soap) {
//                soap_copy_stream(reply_soap, soap);
//                soap_clr_omode(reply_soap, SOAP_ENC_MIME | SOAP_ENC_DIME | SOAP_ENC_MTOM);
//                soap_free_stream(soap); /* prevents close in soap_connect() below */
//                newheader->SOAP_WSA(FaultTo) = oldheader->SOAP_WSA(FaultTo);
//                soap->header = newheader;
//                if (soap_connect(soap, newheader->SOAP_WSA(To), newheader->SOAP_WSA(Action))) {
//                    int err;
//                    soap_copy_stream(soap, reply_soap);
//#if defined(SOAP_WSA_2005)
//                    err = soap_wsa_error(soap, SOAP_WSA(DestinationUnreachable), newheader->SOAP_WSA(To));
//#elif defined(SOAP_WSA_2003)
//                    err = soap_wsa_error(soap, "WS-Addessing destination unreachable");
//#else
//                    err = soap_wsa_error(soap, SOAP_WSA(DestinationUnreachable));
//#endif
//                    soap_free_stream(reply_soap);
//                    soap_end(reply_soap);
//                    soap_free(reply_soap);
//                    soap->header = NULL;
//                    return err;
//                }
//                if (soap_valid_socket(reply_soap->socket)) {
//                    soap_send_empty_response(reply_soap, SOAP_OK);	/* HTTP ACCEPTED */
//                    soap_closesock(reply_soap);
//                }
//                soap_free_stream(reply_soap);
//                soap_end(reply_soap);
//                soap_free(reply_soap);
//                data->fresponse = soap->fresponse;
//                soap->fresponse = soap_wsa_response;	/* response will be a POST */
//            }
//        }
//    } else if (oldheader && oldheader->SOAP_WSA(From))
//        newheader->SOAP_WSA(To) = oldheader->SOAP_WSA(From)->Address;
//    else
//        newheader->SOAP_WSA(To) = (char*)soap_wsa_anonymousURI;

//    soap->header = newheader;
//    soap->action = newheader->SOAP_WSA(Action);

    return SOAP_OK;
}

void Wsa::setWsaString(char** pchWsaString, const std::string & value) {
    *pchWsaString = (char *)soap_malloc(soap_, value.size());
    strcpy( *pchWsaString, value.c_str() );
}

int Wsa::request(const std::string & to, const std::string & action)
{
    if (allocHeader() != 0)
        return soap_->error;

    setWsaString( &soap_->header->SOAP_WSA(MessageID), randUuid(soap_) );
    setWsaString( &soap_->header->SOAP_WSA(To), to.empty() ? ANONYMOUS_URI : to );
    setWsaString( &soap_->header->SOAP_WSA(Action), action);

    soap_->header->SOAP_WSA(RelatesTo) = NULL;
    soap_->header->SOAP_WSA(From) = NULL;
    soap_->header->SOAP_WSA(ReplyTo) = NULL;
    soap_->header->SOAP_WSA(FaultTo) = NULL;
    soap_->header->wsa5__MessageID = NULL;
    soap_->header->wsa5__RelatesTo = NULL;
    soap_->header->wsa5__From = NULL;
    soap_->header->wsa5__ReplyTo = NULL;
    soap_->header->wsa5__FaultTo = NULL;
    soap_->header->wsa5__To = NULL;
    soap_->header->wsa5__Action = NULL;
    return check();
}
