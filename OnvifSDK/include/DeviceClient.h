#ifndef deviceClient_H
#define deviceClient_H

#include "OnvifSDK.h"

#ifdef DEV_S
#include "sigrlog.h"
#include "WebDeviceBindingProxy.h"
#include "WebMediaBindingProxy.h"

class DeviceClient:
        public IOnvifDevMgmt {
public:
	DeviceClient(const char * pchAdress, soap * s);
    virtual ~DeviceClient() {
    }

    int SetDateAndTime( DevSetSystemDateAndTime & );
	int GetDateAndTime(DevGetSystemDateAndTimeResponse &);
    int GetUsers(DevGetUsersResponse &);

private:
	DeviceBindingProxy m_proxy;
};

class MediaClient:
		public IOnvifMedia {
public:
	MediaClient(const char * pchAdress, soap * s);
	virtual ~MediaClient() {

	}

	 int GetProfile(const std::string & profileToken, MedGetProfileResponse & resp) ;
	 int GetProfiles(MedGetProfilesResponse &) ;
	 int GetVideoSources(MedGetVideoSourcesResponse &) ;
	 int GetStreamUri( const std::string& token, std::string & uri) ;
	 int GetCompatibleVideoEncoderConfigurations( MedGetCompatibleVideoEncoderConfigurationsResponse& r ) ;
	 int GetCompatibleVideoAnalyticsConfigurations( MedGetCompatibleVideoAnalyticsConfigurationsResponse& r ) ;
	 int AddVideoAnalyticsConfiguration( const std::string& profileToken,
	                                                const std::string& confToken ) ;
private:
	MediaBindingProxy m_proxy;
};
#endif //DEV_S
#endif	// deviceClient_H

