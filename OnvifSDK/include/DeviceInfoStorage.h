#ifndef DEVICE_INFO_STORAGE__H
#define DEVICE_INFO_STORAGE__H

#include "helpers.h"
#include "OnvifSDK.h"

class DeviceInfoStorage {
    public:
        DeviceInfoStorage(){}
        ~DeviceInfoStorage(){}
        int SetDeviceInfo( OnvifDevice::Type type,
                           const std::string & manufacturer,
                           const std::string & model,
                           const std::string & firmwareVersion,
                           const std::string & serialNumber,
                           const std::string & hardwareId,
                           const std::string & scopes,
                           const std::string & interface,
                           int webservicePort );
        static std::string ResolveType( OnvifDevice::Type type );
        static std::string getInterfaceIp( const std::string & interface );

        int m_webservicePort;
        std::string m_manufacturer;
        std::string m_model;
        std::string m_firmwareVersion;
        std::string m_serialNumber;
        std::string m_hardwareId;
        std::string m_deviceServiceXAddr;
        std::vector<std::string> m_scopes;
        std::string m_endpoint;
        std::string m_type;
        OnvifDevice::Type m_deviceType;
};
#endif //DEVICE_INFO_STORAGE__H
