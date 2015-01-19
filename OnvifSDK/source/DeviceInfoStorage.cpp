
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>

#include "sigrlog.h"
#include "DeviceInfoStorage.h"

std::string
DeviceInfoStorage::getInterfaceIp( const std::string & interface ) {
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;
    getifaddrs(&ifAddrStruct);
    std::string result("");

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa ->ifa_addr->sa_family==AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            SIGRLOG( SIGRDEBUG3, "%s IP Address %s", ifa->ifa_name, addressBuffer);
            if(! strcmp(ifa->ifa_name, interface.c_str()))
                result = std::string(addressBuffer);
        } else if (ifa->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            SIGRLOG( SIGRDEBUG3, "%s IP Address %s", ifa->ifa_name, addressBuffer);
            if( ( !strcmp(ifa->ifa_name, interface.c_str()) ) && result.empty() )
                result = std::string(addressBuffer);
        }
    }

    if( ifAddrStruct != NULL )
        freeifaddrs(ifAddrStruct);

    return result;
}

std::string
DeviceInfoStorage::ResolveType( OnvifDevice::Type type ) {
    switch (type) {
    case OnvifDevice::NVT:
        return "dn:NetworkVideoTransmitter";
        break;
    case OnvifDevice::NVD:
        return "NetworkVideoDisplay";
        break;
    case OnvifDevice::NVR:
        return "NetworkVideoRecorder";
        break;
    default:
    case OnvifDevice::Default:
        return "Device";
        break;
    };
}

int
DeviceInfoStorage::SetDeviceInfo( OnvifDevice::Type type,
                                  const std::string & manufacturer,
                                  const std::string & model,
                                  const std::string & firmwareVersion,
                                  const std::string & serialNumber,
                                  const std::string & hardwareId,
                                  const std::string & scopes,
                                  const std::string & interface,
                                  int webservicePort ) {
    m_deviceType = type;
    m_type = DeviceInfoStorage::ResolveType(type);
    m_manufacturer = manufacturer;
    m_model = model;
    m_firmwareVersion = firmwareVersion;
    m_serialNumber = serialNumber;
    m_hardwareId = hardwareId;
    m_deviceServiceXAddr = DeviceInfoStorage::getInterfaceIp( interface );
    m_scopes = split( scopes, ' ' );
    m_webservicePort = webservicePort;

    std::stringstream ss;
    ss << "http://" <<  m_deviceServiceXAddr << ":" << m_webservicePort;
    m_endpoint = ss.str();
    if( m_deviceServiceXAddr.empty() ) {
        SIGRLOG(SIGRCRITICAL, "DeviceInfoStorage::SetDeviceInfo "
                "hasnt found address for %s interface", interface.c_str() );
        return -1;
    }

    return 0;
}
