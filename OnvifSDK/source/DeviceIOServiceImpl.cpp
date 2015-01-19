
#include "DeviceIOServiceImpl.h"
#ifdef DEVIO_S
#include "BaseServer.h"
#include "sigrlog.h"

#warning TODO for while not supporting copy
DeviceIOBindingService* DeviceIOServiceImpl::copy()
{
	return NULL;
}

int
DeviceIOServiceImpl::GetVideoOutputs( _tmd__GetVideoOutputs *tmd__GetVideoOutputs,
                                      _tmd__GetVideoOutputsResponse *tmd__GetVideoOutputsResponse ) {
    DevIOGetVideoOutputsResponse resp(tmd__GetVideoOutputsResponse);
    int nRes = handler_->GetVideoOutputs(resp);
    CHECKRETURN(nRes, "DeviceIOServiceImpl::GetVideoOutputs");
}

#endif //DEVIO_S
