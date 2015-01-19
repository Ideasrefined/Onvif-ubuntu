#ifndef DEVICEIOCLIENT_H
#define DEVICEIOCLIENT_H

#include "sigrlog.h"
#include "OnvifSDK.h"
#ifdef DEVIO_S

#include "WebDeviceIOBindingProxy.h"

class DeviceIOClient
{
public:
	DeviceIOClient(const char * pchAdress, soap * s);
	~DeviceIOClient();

	int GetVideoOutputs(DevIOGetVideoOutputsResponse &);

private:
	DeviceIOBindingProxy m_proxy;
};
#endif //DEVIO_S
#endif	// DEVICEIOCLIENT_H
