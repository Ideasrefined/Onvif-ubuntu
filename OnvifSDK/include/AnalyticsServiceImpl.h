#ifndef ANALYTICS_IMPL_H__
#define ANALYTICS_IMPL_H__

#include "OnvifSDK.h"

#ifdef ANALY_S
#include "onvifService.h"
#include "WebRuleEngineBindingService.h"

class BaseServer;

class AnalyticsServiceImpl:
        public IOnvifService,
        public RuleEngineBindingService
{
private:
    IOnvifAnalytics* handler_;
    BaseServer* baseServer_;
public:
    AnalyticsServiceImpl( BaseServer* server,
                          IOnvifAnalytics* handler,
                          struct soap* data ):
        RuleEngineBindingService(data),
        baseServer_(server),
        handler_(handler) {
    }

    AnalyticsServiceImpl *copy() {
        return NULL;
    }

    virtual int dispatch() {
        return RuleEngineBindingService::dispatch();
    }

    virtual void destroy() {
        RuleEngineBindingService::destroy();
    }

    virtual	int GetSupportedRules(_tan__GetSupportedRules *tan__GetSupportedRules, _tan__GetSupportedRulesResponse *tan__GetSupportedRulesResponse) { return SOAP_ERR; }

    /// Web service operation 'CreateRules' (returns error code or SOAP_OK)
    virtual	int CreateRules(_tan__CreateRules *tan__CreateRules, _tan__CreateRulesResponse *tan__CreateRulesResponse) { return SOAP_ERR; }

    /// Web service operation 'DeleteRules' (returns error code or SOAP_OK)
    virtual	int DeleteRules(_tan__DeleteRules *tan__DeleteRules, _tan__DeleteRulesResponse *tan__DeleteRulesResponse) { return SOAP_ERR; }

    /// Web service operation 'GetRules' (returns error code or SOAP_OK)
    virtual	int GetRules(_tan__GetRules *tan__GetRules, _tan__GetRulesResponse *tan__GetRulesResponse) { return SOAP_ERR; }

    /// Web service operation 'ModifyRules' (returns error code or SOAP_OK)
    virtual	int ModifyRules(_tan__ModifyRules *tan__ModifyRules, _tan__ModifyRulesResponse *tan__ModifyRulesResponse) { return SOAP_ERR; }

    /// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
    virtual	int GetServiceCapabilities(_tan__GetServiceCapabilities *tan__GetServiceCapabilities, _tan__GetServiceCapabilitiesResponse *tan__GetServiceCapabilitiesResponse) { return SOAP_ERR; }

    /// Web service operation 'GetSupportedAnalyticsModules' (returns error code or SOAP_OK)
    virtual	int GetSupportedAnalyticsModules(_tan__GetSupportedAnalyticsModules *tan__GetSupportedAnalyticsModules, _tan__GetSupportedAnalyticsModulesResponse *tan__GetSupportedAnalyticsModulesResponse);

    /// Web service operation 'CreateAnalyticsModules' (returns error code or SOAP_OK)
    virtual	int CreateAnalyticsModules(_tan__CreateAnalyticsModules *tan__CreateAnalyticsModules, _tan__CreateAnalyticsModulesResponse *tan__CreateAnalyticsModulesResponse) { return SOAP_ERR; }

    /// Web service operation 'DeleteAnalyticsModules' (returns error code or SOAP_OK)
    virtual	int DeleteAnalyticsModules(_tan__DeleteAnalyticsModules *tan__DeleteAnalyticsModules, _tan__DeleteAnalyticsModulesResponse *tan__DeleteAnalyticsModulesResponse) { return SOAP_ERR; }

    /// Web service operation 'GetAnalyticsModules' (returns error code or SOAP_OK)
    virtual	int GetAnalyticsModules(_tan__GetAnalyticsModules *tan__GetAnalyticsModules, _tan__GetAnalyticsModulesResponse *tan__GetAnalyticsModulesResponse);

    /// Web service operation 'ModifyAnalyticsModules' (returns error code or SOAP_OK)
    virtual	int ModifyAnalyticsModules(_tan__ModifyAnalyticsModules *tan__ModifyAnalyticsModules, _tan__ModifyAnalyticsModulesResponse *tan__ModifyAnalyticsModulesResponse) { return SOAP_ERR; }
};

#endif // ANALY_S
#endif // ANALYTICS_IMPL_H__
