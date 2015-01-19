
#include "DeviceClient.h"

#ifdef DEV_S
DeviceClient::DeviceClient(const char * pchAdress, soap * s):m_proxy(s)
{
	m_proxy.soap_endpoint = pchAdress;
}

int DeviceClient::GetDateAndTime(DevGetSystemDateAndTimeResponse & resp)
{
	DevGetSystemDateAndTime req(m_proxy.soap);

	int nRes = m_proxy.GetSystemDateAndTime(req.d, resp.d);

	CHECKRETURN(nRes, "DeviceClient::GetDateAndTime");
}


int DeviceClient::SetDateAndTime(DevSetSystemDateAndTime & req)
{
    DevSetSystemDateAndTimeResponse resp(m_proxy.soap);

	int nRes = m_proxy.SetSystemDateAndTime(req.d, resp.d);

	CHECKRETURN(nRes, "DeviceClient::SetDateAndTime");
}

int DeviceClient::GetUsers(DevGetUsersResponse & resp)
{
	DevGetUsers req(m_proxy.soap);

	int nRes = m_proxy.GetUsers(req.d, resp.d);

	CHECKRETURN(nRes, "DeviceClient::GetUsers");
}

MediaClient::MediaClient(const char * pchAdress, soap * s):m_proxy(s)
{
	m_proxy.soap_endpoint = pchAdress;
}
int MediaClient::GetProfile(const std::string & profileToken, MedGetProfileResponse & resp)
{

}
int MediaClient::GetProfiles(MedGetProfilesResponse &resp)
{
	_trt__GetProfiles gp;
	int profiles_found = 0;

	int nRes = m_proxy.GetProfiles(&gp,resp.d);
	profiles_found = resp.d->Profiles.size();
	printf("Profies : %d \n", profiles_found);
	/*for(int i = 0 ; i < profiles_found; i++)
	{
		printf("PRF:%d -> TK:%s\n->NM:%s\n",i,resp.d->Profiles[i]->token.c_str(),resp.d->Profiles[i]->Name.c_str());
	}*/

	CHECKRETURN(nRes, "MediaClient::GetProfiles");

}
int MediaClient::GetVideoSources(MedGetVideoSourcesResponse &resp)
{

}

int MediaClient::GetStreamUri(const std::string& token, std::string & uri)
{
	_trt__GetStreamUri arg1;
	_trt__GetStreamUriResponse arg2;
	tt__StreamSetup ss;
	tt__Transport	tt;
	tt.Protocol = tt__TransportProtocol__RTSP;
	ss.Stream = tt__StreamType__RTP_Unicast;
	ss.Transport = &tt;

	arg1.ProfileToken = token;
	arg1.StreamSetup = &ss;

	//printf(">>>inside GetStreamURI\n");
	int nRes = m_proxy.GetStreamUri(&arg1,&arg2);
	//printf(">>>getStreamResult : %d \n",nRes);

	//printf(">>>Stream Requested : %s\n",arg2.MediaUri->Uri.c_str());
	//token = arg1.ProfileToken;
	uri = arg2.MediaUri->Uri;
	//printf(">>>RESPONSE COPIED \n \n");

	CHECKRETURN(nRes, "MediaClient::GetStreamUri");

}
int MediaClient::GetCompatibleVideoEncoderConfigurations( MedGetCompatibleVideoEncoderConfigurationsResponse& r )
{

}
int MediaClient::GetCompatibleVideoAnalyticsConfigurations( MedGetCompatibleVideoAnalyticsConfigurationsResponse& r )
{

}
int MediaClient::AddVideoAnalyticsConfiguration( const std::string& profileToken,
	                                                const std::string& confToken )
{

}
#endif //DEV_S
