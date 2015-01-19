#ifndef ONVIFTESTSERVER_H__
#define ONVIFTESTSERVER_H__

#include "OnvifSDK.h"

class OnvifTestServer :
        public IOnvifDevMgmt
{
public:
    virtual int GetDateAndTime( DevGetSystemDateAndTimeResponse & );
    virtual int SetDateAndTime( DevSetSystemDateAndTime & ) { return -1; }
    virtual int GetUsers( DevGetUsersResponse & );
};

#endif	// ONVIFTESTSERVER_H__
