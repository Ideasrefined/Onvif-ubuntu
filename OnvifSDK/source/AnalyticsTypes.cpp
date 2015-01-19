
#include "OnvifSDK.h"
#ifdef ANALY_S
#include "WebH.h"
#include "commonTypes.h"


#define EXTRA_CONSTRUCT() \
{\
}

CLASS_CTORS(tan, Anlt, GetAnalyticsModulesResponse)


void
AnltGetAnalyticsModulesResponse::setModule( std::vector<tt__Config * > & m ) {
    d->AnalyticsModule = m;
}

#endif //ANALY_S
