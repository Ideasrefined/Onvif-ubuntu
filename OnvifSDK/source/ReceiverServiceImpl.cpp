
#include "OnvifSDK.h"
#ifdef RECV_S
#include "sigrlog.h"
#include "ReceiverServiceImpl.h"
#include "BaseServer.h"

#warning TODO for while not supporting copy
ReceiverBindingService* ReceiverServiceImpl::copy()
{
    return NULL;
}

int ReceiverServiceImpl::GetReceivers(_trv__GetReceivers *trv__GetReceivers, _trv__GetReceiversResponse *trv__GetReceiversResponse)
{
	RecvGetReceiversResponse resp( trv__GetReceiversResponse );
    int nRes = handler_->GetReceivers(resp);
	CHECKRETURN(nRes, "ReceiverServiceImpl::GetReceivers");
}

int ReceiverServiceImpl::CreateReceiver(_trv__CreateReceiver *trv__CreateReceiver, _trv__CreateReceiverResponse *trv__CreateReceiverResponse)
{
    RecvCreateReceiver req(trv__CreateReceiver);
    RecvCreateReceiverResponse resp(trv__CreateReceiverResponse);

    std::string recvToken;
    int nRes = handler_->CreateReceiver( req.getUri(), recvToken );
    resp.setToken(recvToken);
    CHECKRETURN(nRes, "ReceiverServiceImpl::CreateReceiver");
}

int ReceiverServiceImpl::SetReceiverMode(_trv__SetReceiverMode *trv__SetReceiverMode, _trv__SetReceiverModeResponse *trv__SetReceiverModeResponse)
{
    RecvSetReceiverMode req(trv__SetReceiverMode);
    int nRes = handler_->SetReceiverMode( req.getToken(), req.getMode() );
    CHECKRETURN(nRes, "ReceiverServiceImpl::SetReceiverMode");
}
#endif // RECV_S
