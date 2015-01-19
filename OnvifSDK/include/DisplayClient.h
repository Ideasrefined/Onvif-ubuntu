#ifndef DISPLAYCLIENT_H
#define DISPLAYCLIENT_H

#include "sigrlog.h"
#include "OnvifSDK.h"

#ifdef DISP_S

#include "WebDisplayBindingProxy.h"

class DisplayClient
{
public:
	DisplayClient(const char * pchAdress, soap * s);
	~DisplayClient();

	int GetLayout(DispGetLayout &, DispGetLayoutResponse &);
    int GetDisplayOptions(DispGetDisplayOptions &, DispGetDisplayOptionsResponse &);
    int SetLayout(DispSetLayout &);
    int CreatePaneConfiguration(DispCreatePaneConfiguration &, DispCreatePaneConfigurationResponse &);

private:
	DisplayBindingProxy m_proxy;
};

#endif // DISP_S
#endif // DISPLAYCLIENT_H

