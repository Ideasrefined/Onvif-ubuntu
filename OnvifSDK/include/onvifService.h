#ifndef ONVIF_SERVICE_H__
#define ONVIF_SERVICE_H__

class IOnvifService {
public:
    virtual	int dispatch() = 0;
    virtual	void destroy() = 0;
    virtual ~IOnvifService() = 0;
};

#endif //ONVIF_SERVICE_H__
