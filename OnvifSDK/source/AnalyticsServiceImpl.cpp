
#include "AnalyticsServiceImpl.h"

#ifdef ANALY_S
#include "BaseServer.h"

int
AnalyticsServiceImpl::GetAnalyticsModules( _tan__GetAnalyticsModules *tan__GetAnalyticsModules,
                                           _tan__GetAnalyticsModulesResponse *tan__GetAnalyticsModulesResponse) {
    AnltGetAnalyticsModulesResponse r(tan__GetAnalyticsModulesResponse);
    return handler_->GetAnalyticsModules( tan__GetAnalyticsModules->ConfigurationToken, r );
}

int
AnalyticsServiceImpl::GetSupportedAnalyticsModules( _tan__GetSupportedAnalyticsModules *tan__GetSupportedAnalyticsModules,
                                                    _tan__GetSupportedAnalyticsModulesResponse *tan__GetSupportedAnalyticsModulesResponse) {
    return 0;
}

#endif //ANALY_S
