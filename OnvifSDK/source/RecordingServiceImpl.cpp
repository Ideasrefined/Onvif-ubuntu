
#include "RecordingServiceImpl.h"
#ifdef RECORD_S
#include "BaseServer.h"


int RecordingServiceImpl::CreateRecordingJob (_trc__CreateRecordingJob *trc__CreateRecordingJob,
                                              _trc__CreateRecordingJobResponse *trc__CreateRecordingJobResponse)
{
    RecCreateRecordingJob req(trc__CreateRecordingJob);
    RecCreateRecordingJobResponse resp(trc__CreateRecordingJobResponse);
    return handler_->CreateRecordingJob(req, resp);
}

int RecordingServiceImpl::DeleteRecordingJob (_trc__DeleteRecordingJob *trc__DeleteRecordingJob,
                                              _trc__DeleteRecordingJobResponse *trc__DeleteRecordingJobResponse)
{
    RecDeleteRecordingJob r(trc__DeleteRecordingJob);
    return handler_->DeleteRecordingJob(r.getToken());
}

int RecordingServiceImpl::CreateRecording (_trc__CreateRecording *trc__CreateRecording,
                                           _trc__CreateRecordingResponse *trc__CreateRecordingResponse)
{
    RecCreateRecording req(trc__CreateRecording);
    RecCreateRecordingResponse resp(trc__CreateRecordingResponse);
    return handler_->CreateRecording(req, resp);
}

int RecordingServiceImpl::DeleteRecording (_trc__DeleteRecording *trc__DeleteRecording,
                                           _trc__DeleteRecordingResponse *trc__DeleteRecordingResponse)
{
    RecDeleteRecording r(trc__DeleteRecording);
    return handler_->DeleteRecording(r.getToken());
}

#endif //RECORD_S
