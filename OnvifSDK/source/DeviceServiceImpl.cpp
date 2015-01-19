#include "DeviceServiceImpl.h"

#ifdef DEV_S
#include "sigrlog.h"
#include "BaseServer.h"

int
DeviceServiceImpl::GetSystemDateAndTime( _tds__GetSystemDateAndTime *tds__GetSystemDateAndTime,
                                         _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse ) {
	DevGetSystemDateAndTimeResponse dt(tds__GetSystemDateAndTimeResponse);
    int nRes = handler_->GetDateAndTime(dt);
	CHECKRETURN(nRes, "DeviceServiceImpl::GetSystemDateAndTime");
}

// for while not supporting copy
DeviceBindingService* DeviceServiceImpl::copy() {
	return NULL;
}

int
DeviceServiceImpl::SetSystemDateAndTime( _tds__SetSystemDateAndTime *tds__SetSystemDateAndTime,
                                         _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse ) {
    if(tds__SetSystemDateAndTime->DateTimeType != tt__SetDateTimeType__Manual) {
		SIGRLOG(SIGRWARNING, "DeviceServiceImpl::SetSystemDateAndTime Time is not Manual");
		return SOAP_ERR;
	}

	DevSetSystemDateAndTime dt(tds__SetSystemDateAndTime);
    int nRes = handler_->SetDateAndTime(dt);
	CHECKRETURN(nRes, "DeviceServiceImpl::SetSystemDateAndTime");
}

int
DeviceServiceImpl::GetCapabilities( _tds__GetCapabilities *tds__GetCapabilities,
                                    _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse ) {
	DevGetCapabilitiesResponse resp(tds__GetCapabilitiesResponse);
    resp.SetDeviceCapabilities( baseServer_->m_endpoint );
    if( baseServer_->m_deviceType == OnvifDevice::NVT ) {
        resp.SetEventsCapabilities( baseServer_->m_endpoint );
        resp.SetMediaCapabilities( baseServer_->m_endpoint );
        resp.SetAnalyticsCapabilities( baseServer_->m_endpoint );
    }

	return SOAP_OK;
}

int
DeviceServiceImpl::GetDeviceInformation( _tds__GetDeviceInformation *tds__GetDeviceInformation,
                                         _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse ) {
	DevGetDeviceInformationResponse resp(tds__GetDeviceInformationResponse);
    resp.SetDeviceInfo( baseServer_->m_manufacturer, baseServer_->m_model,
                        baseServer_->m_firmwareVersion, baseServer_->m_serialNumber, baseServer_->m_hardwareId );

	return SOAP_OK;
}

int
DeviceServiceImpl::GetUsers( _tds__GetUsers *tds__GetUsers,
                             _tds__GetUsersResponse *tds__GetUsersResponse) {
	DevGetUsersResponse resp(tds__GetUsersResponse);
    int nRes = handler_->GetUsers(resp);
	CHECKRETURN(nRes, "DeviceServiceImpl::GetUsers");
}

int
DeviceServiceImpl::GetServices( _tds__GetServices *tds__GetServices,
                                _tds__GetServicesResponse *tds__GetServicesResponse ) {
	DevGetServices req(tds__GetServices);
    DevGetServicesResponse resp(tds__GetServicesResponse);
    if( req.d->IncludeCapability )
        SIGRLOG(SIGRWARNING, "DeviceServiceImpl::GetServices 'Including Capabilities' still not implemented");

    resp.AddService( "http://www.onvif.org/ver10/events/wsdl",  baseServer_->m_endpoint, 1 );
    resp.AddService( "http://www.onvif.org/ver20/analytics/wsdl",  baseServer_->m_endpoint, 2 );
	return 0;
}

int
DeviceServiceImpl::GetScopes( _tds__GetScopes *tds__GetScopes,
                              _tds__GetScopesResponse *tds__GetScopesResponse ) {
    DevGetScopesResponse resp(tds__GetScopesResponse);
    resp.AddItems( baseServer_->m_scopes );
    return SOAP_OK;
}

#endif //DEV_S
