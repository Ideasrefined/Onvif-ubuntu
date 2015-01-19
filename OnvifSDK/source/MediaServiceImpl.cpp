
#include "MediaServiceImpl.h"
#include "BaseServer.h"

int
MediaServiceImpl::GetStreamUri( _trt__GetStreamUri *trt__GetStreamUri,
                                _trt__GetStreamUriResponse *trt__GetStreamUriResponse) {

    trt__GetStreamUriResponse->MediaUri = soap_new_tt__MediaUri( soap );
    if( SOAP_OK != handler_->GetStreamUri( trt__GetStreamUri->ProfileToken,
                                                trt__GetStreamUriResponse->MediaUri->Uri )  )
        return SOAP_ERR;

    trt__GetStreamUriResponse->MediaUri->InvalidAfterConnect = false;
    trt__GetStreamUriResponse->MediaUri->InvalidAfterReboot = false;
    trt__GetStreamUriResponse->MediaUri->Timeout = "PT60S";

    return SOAP_OK;
}

int
MediaServiceImpl::GetVideoSources( _trt__GetVideoSources *trt__GetVideoSources,
                                   _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse ) {
    MedGetVideoSourcesResponse r( trt__GetVideoSourcesResponse );
    return handler_->GetVideoSources(r);
}

int
MediaServiceImpl::GetProfiles( _trt__GetProfiles *trt__GetProfiles,
                               _trt__GetProfilesResponse *trt__GetProfilesResponse ) {
    MedGetProfilesResponse r( trt__GetProfilesResponse );
    return handler_->GetProfiles(r);
}

int
MediaServiceImpl::GetProfile( _trt__GetProfile *trt__GetProfile,
                              _trt__GetProfileResponse *trt__GetProfileResponse) {
    MedGetProfileResponse r( trt__GetProfileResponse );
    return handler_->GetProfile( trt__GetProfile->ProfileToken, r );
}

int
MediaServiceImpl::GetSnapshotUri( _trt__GetSnapshotUri *trt__GetSnapshotUri,
                                  _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse) {
    trt__GetSnapshotUriResponse->MediaUri = soap_new_tt__MediaUri( soap );
    trt__GetSnapshotUriResponse->MediaUri->Uri = "http://192.168.10.23/sdget.htm?FILE=snapshot.jpg";
    trt__GetSnapshotUriResponse->MediaUri->InvalidAfterConnect = false;
    trt__GetSnapshotUriResponse->MediaUri->InvalidAfterReboot = false;
    trt__GetSnapshotUriResponse->MediaUri->Timeout = "PT60S";
    return SOAP_OK;
}

int
MediaServiceImpl::GetCompatibleVideoEncoderConfigurations( _trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations,
                                                           _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse) {
    MedGetCompatibleVideoEncoderConfigurationsResponse r(trt__GetCompatibleVideoEncoderConfigurationsResponse);
    return handler_->GetCompatibleVideoEncoderConfigurations(r);
}

int
MediaServiceImpl::GetCompatibleVideoAnalyticsConfigurations( _trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations,
                                                             _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse) {
    MedGetCompatibleVideoAnalyticsConfigurationsResponse r(trt__GetCompatibleVideoAnalyticsConfigurationsResponse);
    return handler_->GetCompatibleVideoAnalyticsConfigurations(r);
}

int
MediaServiceImpl::AddVideoAnalyticsConfiguration( _trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration,
                                                  _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse) {
    return handler_->AddVideoAnalyticsConfiguration( trt__AddVideoAnalyticsConfiguration->ProfileToken,
                                                          trt__AddVideoAnalyticsConfiguration->ConfigurationToken );
}
