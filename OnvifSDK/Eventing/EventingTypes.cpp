
#include "sigrlog.h"
#include "OnvifSDK.h"
#include "commonTypes.h"
#include "WebPullPointSubscriptionBindingProxy.h"

    #define EXTRA_CONSTRUCT() \
    {\
    }

    CLASS_CTORS(wsnt, Evnt, Subscribe)

    void EvntSubscribe::setAddress(const std::string & address)
    {
        d->ConsumerReference.Address = (char*)soap_malloc( d->soap, sizeof(char) * address.length() );
        strcpy( d->ConsumerReference.Address, address.c_str() );
    }


    CLASS_CTORS(wsnt, Evnt, SubscribeResponse)
    CLASS_CTORS(wsnt, Evnt, Notify)

