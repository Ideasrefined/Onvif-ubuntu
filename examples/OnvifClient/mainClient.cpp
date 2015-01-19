
#include <stdio.h>
#include "OnvifSDK.h"
//#include "WebMediaBindingProxy.h"
//#include <conio.h>

void notsCatcher( const char * msgText) {
    printf("Client code got the Event message: [%s]\n", msgText);
}

int main(const int argc, const char* argv[])
{
    IOnvifClient * pClient = getOnvifClient();
    std::string camIP;
    FILE *fp;

    std::vector<DiscoveryMatch> devices = pClient->DiscoverDevices( OnvifDevice::NVT );
    if(devices.size() == 0) {
        printf("Disappointment. 0 devices were discovered\n");
        return -1;
    } else {
        printf("%d devices found \n",devices.size());
        for(int i = 0; i < devices.size(); i++)
        {    printf("Discovered device: %s\n", devices[i].xaddr_.c_str());
        	camIP = devices[i].xaddr_;

        }

    }
    if(devices.size() > 0)
    {
        fp = fopen("camlist.txt","w");
    }

    for(int i = 0 ; i < devices.size(); i++)
    {
        if( pClient->Init( devices[i].xaddr_.c_str() ) != 0)
            return -1;
        if( !pClient->SetNotificationCatcher( notsCatcher ) ) {
            return -1;
        }

        //update cam ip wrt array
        camIP = devices[i].xaddr_;

        soap* pSoap = pClient->GetSoap();
        IOnvifDevMgmt* devMgmt = pClient->getDeviceClient();
        DevGetSystemDateAndTimeResponse r(pSoap);
        int iRet = devMgmt->GetDateAndTime(r);
        if(iRet != 0) {
            printf("GetDateAndTime failed\n");
            return -1;
        } else {
             int nYear, nMonth, nDay, nHour, nMin, nSec;
             r.GetUTCDateAndTime(nYear, nMonth, nDay, nHour, nMin, nSec);
             printf("DeviceService response is:\nDate is %d:%d:%d\nTime is %d:%d:%d\n", nYear, nMonth, nDay, nHour, nMin, nSec);
        }

        //soap* pSoap2 = pClient->GetSoap();
        printf("\n\n\n >>>> SUPER MAN DOING DOING \n %s \n",camIP.c_str());



        IOnvifMedia* med = pClient->getMediaClient();
        if(med == NULL)
        {
        	printf("NO MEDIA CLIENT FOUND \n");
        	return 0;
        }

        printf("\n\n\n >>>> STEP 2 \n");

        MedGetProfilesResponse profilesResponse(pSoap);

        med->GetProfiles(profilesResponse);

        std::string tokensArray[4];
        profilesResponse.GetProfilesTokens(tokensArray);

        for(int i = 0 ; i < 4; i ++)
        {
        	printf("Token%d: %s\n",i,tokensArray[i].c_str());
        }


        std::string tk,uri;
        tk = tokensArray[0];
        med->GetStreamUri(tk,uri);

        printf(">>>Stream Requested : %s\n",uri.c_str());
        fprintf(fp,"%s\n",uri.c_str());
        //system("echo mango > textfile");
    }
    fflush(stdout);
    fclose(fp);
    printf("\n\n\n >>>> STEP 3 \n");
    return 0;
}

