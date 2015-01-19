/* WebRuleEngineBindingProxy.h
   Generated by gSOAP 2.8.15 from web.h

Copyright(C) 2000-2013, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under ONE of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef WebRuleEngineBindingProxy_H
#define WebRuleEngineBindingProxy_H
#include "WebH.h"

class SOAP_CMAC RuleEngineBindingProxy
{ public:
	struct soap *soap;
	bool own;
	/// Endpoint URL of service 'RuleEngineBindingProxy' (change as needed)
	const char *soap_endpoint;
	/// Constructor
	RuleEngineBindingProxy();
	/// Constructor to use/share an engine state
	RuleEngineBindingProxy(struct soap*);
	/// Constructor with endpoint URL
	RuleEngineBindingProxy(const char *url);
	/// Constructor with engine input+output mode control
	RuleEngineBindingProxy(soap_mode iomode);
	/// Constructor with URL and input+output mode control
	RuleEngineBindingProxy(const char *url, soap_mode iomode);
	/// Constructor with engine input and output mode control
	RuleEngineBindingProxy(soap_mode imode, soap_mode omode);
	/// Destructor frees deserialized data
	virtual	~RuleEngineBindingProxy();
	/// Initializer used by constructors
	virtual	void RuleEngineBindingProxy_init(soap_mode imode, soap_mode omode);
	/// Delete all deserialized data (with soap_destroy and soap_end)
	virtual	void destroy();
	/// Delete all deserialized data and reset to default
	virtual	void reset();
	/// Disables and removes SOAP Header from message
	virtual	void soap_noheader();
	/// Put SOAP Header in message
	virtual	void soap_header(char *wsa5__MessageID, struct wsa5__RelatesToType *wsa5__RelatesTo, struct wsa5__EndpointReferenceType *wsa5__From, struct wsa5__EndpointReferenceType *wsa5__ReplyTo, struct wsa5__EndpointReferenceType *wsa5__FaultTo, char *wsa5__To, char *wsa5__Action, struct chan__ChannelInstanceType *chan__ChannelInstance, char *wsa__MessageID, wsa__Relationship *wsa__RelatesTo, wsa__EndpointReferenceType *wsa__From, wsa__EndpointReferenceType *wsa__ReplyTo, wsa__EndpointReferenceType *wsa__FaultTo, char *wsa__To, char *wsa__Action, wsd__AppSequenceType *wsd__AppSequence);
	/// Get SOAP Header structure (NULL when absent)
	virtual	const SOAP_ENV__Header *soap_header();
	/// Get SOAP Fault structure (NULL when absent)
	virtual	const SOAP_ENV__Fault *soap_fault();
	/// Get SOAP Fault string (NULL when absent)
	virtual	const char *soap_fault_string();
	/// Get SOAP Fault detail as string (NULL when absent)
	virtual	const char *soap_fault_detail();
	/// Close connection (normally automatic, except for send_X ops)
	virtual	int soap_close_socket();
	/// Force close connection (can kill a thread blocked on IO)
	virtual	int soap_force_close_socket();
	/// Print fault
	virtual	void soap_print_fault(FILE*);
#ifndef WITH_LEAN
	/// Print fault to stream
#ifndef WITH_COMPAT
	virtual	void soap_stream_fault(std::ostream&);
#endif

	/// Put fault into buffer
	virtual	char *soap_sprint_fault(char *buf, size_t len);
#endif

	/// Web service operation 'GetSupportedRules' (returns error code or SOAP_OK)
	virtual	int GetSupportedRules(_tan__GetSupportedRules *tan__GetSupportedRules, _tan__GetSupportedRulesResponse *tan__GetSupportedRulesResponse) { return this->GetSupportedRules(NULL, NULL, tan__GetSupportedRules, tan__GetSupportedRulesResponse); }
	virtual	int GetSupportedRules(const char *endpoint, const char *soap_action, _tan__GetSupportedRules *tan__GetSupportedRules, _tan__GetSupportedRulesResponse *tan__GetSupportedRulesResponse);

	/// Web service operation 'CreateRules' (returns error code or SOAP_OK)
	virtual	int CreateRules(_tan__CreateRules *tan__CreateRules, _tan__CreateRulesResponse *tan__CreateRulesResponse) { return this->CreateRules(NULL, NULL, tan__CreateRules, tan__CreateRulesResponse); }
	virtual	int CreateRules(const char *endpoint, const char *soap_action, _tan__CreateRules *tan__CreateRules, _tan__CreateRulesResponse *tan__CreateRulesResponse);

	/// Web service operation 'DeleteRules' (returns error code or SOAP_OK)
	virtual	int DeleteRules(_tan__DeleteRules *tan__DeleteRules, _tan__DeleteRulesResponse *tan__DeleteRulesResponse) { return this->DeleteRules(NULL, NULL, tan__DeleteRules, tan__DeleteRulesResponse); }
	virtual	int DeleteRules(const char *endpoint, const char *soap_action, _tan__DeleteRules *tan__DeleteRules, _tan__DeleteRulesResponse *tan__DeleteRulesResponse);

	/// Web service operation 'GetRules' (returns error code or SOAP_OK)
	virtual	int GetRules(_tan__GetRules *tan__GetRules, _tan__GetRulesResponse *tan__GetRulesResponse) { return this->GetRules(NULL, NULL, tan__GetRules, tan__GetRulesResponse); }
	virtual	int GetRules(const char *endpoint, const char *soap_action, _tan__GetRules *tan__GetRules, _tan__GetRulesResponse *tan__GetRulesResponse);

	/// Web service operation 'ModifyRules' (returns error code or SOAP_OK)
	virtual	int ModifyRules(_tan__ModifyRules *tan__ModifyRules, _tan__ModifyRulesResponse *tan__ModifyRulesResponse) { return this->ModifyRules(NULL, NULL, tan__ModifyRules, tan__ModifyRulesResponse); }
	virtual	int ModifyRules(const char *endpoint, const char *soap_action, _tan__ModifyRules *tan__ModifyRules, _tan__ModifyRulesResponse *tan__ModifyRulesResponse);

	/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
	virtual	int GetServiceCapabilities(_tan__GetServiceCapabilities *tan__GetServiceCapabilities, _tan__GetServiceCapabilitiesResponse *tan__GetServiceCapabilitiesResponse) { return this->GetServiceCapabilities(NULL, NULL, tan__GetServiceCapabilities, tan__GetServiceCapabilitiesResponse); }
	virtual	int GetServiceCapabilities(const char *endpoint, const char *soap_action, _tan__GetServiceCapabilities *tan__GetServiceCapabilities, _tan__GetServiceCapabilitiesResponse *tan__GetServiceCapabilitiesResponse);

	/// Web service operation 'GetSupportedAnalyticsModules' (returns error code or SOAP_OK)
	virtual	int GetSupportedAnalyticsModules(_tan__GetSupportedAnalyticsModules *tan__GetSupportedAnalyticsModules, _tan__GetSupportedAnalyticsModulesResponse *tan__GetSupportedAnalyticsModulesResponse) { return this->GetSupportedAnalyticsModules(NULL, NULL, tan__GetSupportedAnalyticsModules, tan__GetSupportedAnalyticsModulesResponse); }
	virtual	int GetSupportedAnalyticsModules(const char *endpoint, const char *soap_action, _tan__GetSupportedAnalyticsModules *tan__GetSupportedAnalyticsModules, _tan__GetSupportedAnalyticsModulesResponse *tan__GetSupportedAnalyticsModulesResponse);

	/// Web service operation 'CreateAnalyticsModules' (returns error code or SOAP_OK)
	virtual	int CreateAnalyticsModules(_tan__CreateAnalyticsModules *tan__CreateAnalyticsModules, _tan__CreateAnalyticsModulesResponse *tan__CreateAnalyticsModulesResponse) { return this->CreateAnalyticsModules(NULL, NULL, tan__CreateAnalyticsModules, tan__CreateAnalyticsModulesResponse); }
	virtual	int CreateAnalyticsModules(const char *endpoint, const char *soap_action, _tan__CreateAnalyticsModules *tan__CreateAnalyticsModules, _tan__CreateAnalyticsModulesResponse *tan__CreateAnalyticsModulesResponse);

	/// Web service operation 'DeleteAnalyticsModules' (returns error code or SOAP_OK)
	virtual	int DeleteAnalyticsModules(_tan__DeleteAnalyticsModules *tan__DeleteAnalyticsModules, _tan__DeleteAnalyticsModulesResponse *tan__DeleteAnalyticsModulesResponse) { return this->DeleteAnalyticsModules(NULL, NULL, tan__DeleteAnalyticsModules, tan__DeleteAnalyticsModulesResponse); }
	virtual	int DeleteAnalyticsModules(const char *endpoint, const char *soap_action, _tan__DeleteAnalyticsModules *tan__DeleteAnalyticsModules, _tan__DeleteAnalyticsModulesResponse *tan__DeleteAnalyticsModulesResponse);

	/// Web service operation 'GetAnalyticsModules' (returns error code or SOAP_OK)
	virtual	int GetAnalyticsModules(_tan__GetAnalyticsModules *tan__GetAnalyticsModules, _tan__GetAnalyticsModulesResponse *tan__GetAnalyticsModulesResponse) { return this->GetAnalyticsModules(NULL, NULL, tan__GetAnalyticsModules, tan__GetAnalyticsModulesResponse); }
	virtual	int GetAnalyticsModules(const char *endpoint, const char *soap_action, _tan__GetAnalyticsModules *tan__GetAnalyticsModules, _tan__GetAnalyticsModulesResponse *tan__GetAnalyticsModulesResponse);

	/// Web service operation 'ModifyAnalyticsModules' (returns error code or SOAP_OK)
	virtual	int ModifyAnalyticsModules(_tan__ModifyAnalyticsModules *tan__ModifyAnalyticsModules, _tan__ModifyAnalyticsModulesResponse *tan__ModifyAnalyticsModulesResponse) { return this->ModifyAnalyticsModules(NULL, NULL, tan__ModifyAnalyticsModules, tan__ModifyAnalyticsModulesResponse); }
	virtual	int ModifyAnalyticsModules(const char *endpoint, const char *soap_action, _tan__ModifyAnalyticsModules *tan__ModifyAnalyticsModules, _tan__ModifyAnalyticsModulesResponse *tan__ModifyAnalyticsModulesResponse);
};
#endif