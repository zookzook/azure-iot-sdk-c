// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <inttypes.h>

#include "serializer_devicetwin.h"
#include "iothub_client.h"
#include "iothubtransportmqtt.h"
#include "azure_c_shared_utility/threadapi.h"
#include "parson.h"
#include "azure_c_shared_utility/platform.h"

/*String containing Hostname, Device Id & Device Key in the format:             */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessKey=<device_key>"    */
static const char* connectionString = "HostName=...";

// Define the Model - it is a car.
BEGIN_NAMESPACE(Whober);

DECLARE_STRUCT(Geo,
    double, longitude,
    double, latitude
);

DECLARE_DEVICETWIN_MODEL(Rider,
    WITH_REPORTED_PROPERTY(ascii_char_ptr, name),
    WITH_REPORTED_PROPERTY(uint8_t, wf_state), /* 0 = hailed, 1 = in transit, 2 = dropped off */
    WITH_REPORTED_PROPERTY(Geo, location),
    WITH_REPORTED_PROPERTY(Geo, destination)
);

END_NAMESPACE(Whober);

void deviceTwinCallback(int status_code, void* userContextCallback)
{
    (void)(userContextCallback);
    printf("IoTHub: reported properties delivered with status_code = %u\n", status_code);
}

int main(int argc, char *argv[])
{
    /*prepare the platform*/
    if (platform_init() != 0)
    {
        printf("Failed to initialize the platform.\n");
    }
    else
    {
        if (SERIALIZER_REGISTER_NAMESPACE(Whober) == NULL)
        {
            LogError("unable to SERIALIZER_REGISTER_NAMESPACE");
        }
        else
        {
            const char *cs = connectionString;
            if (argc > 1)
            {
                cs = argv[1];
            }

            /*create an IoTHub client*/
            IOTHUB_CLIENT_HANDLE iotHubClientHandle = IoTHubClient_CreateFromConnectionString(cs, MQTT_Protocol);
            if (iotHubClientHandle == NULL)
            {
                printf("Failure creating IoTHubClient handle");
            }
            else
            {
                // Turn on Log 
                bool trace = true;
                (void)IoTHubClient_SetOption(iotHubClientHandle, "logtrace", &trace);

                Rider* rider = IoTHubDeviceTwin_CreateRider(iotHubClientHandle);
                if (rider == NULL)
                {
                    printf("Failure in IoTHubDeviceTwin_CreateCar");
                }
                else
                {
                    /*setting values for reported properties*/
                    rider->name = "Oliver";
                    rider->location.latitude = 11.0;
                    rider->location.longitude = 22.0;
                    rider->destination.latitude = 22.0;
                    rider->destination.longitude = 33.0;
                    rider->wf_state = 0;

                    /*sending the values to IoTHub*/
                    if (IoTHubDeviceTwin_SendReportedStateRider(rider, deviceTwinCallback, NULL) != IOTHUB_CLIENT_OK)
                    {
                        (void)printf("Failed sending serialized reported state\n");
                    }
                    else
                    {
                        printf("Reported state will be sent to IoTHub\n");
                    }

                    printf("press ENTER to end the sample\n");
                    (void)getchar();

                }
                IoTHubDeviceTwin_DestroyRider(rider);
            }
            IoTHubClient_Destroy(iotHubClientHandle);
        }
    }
    platform_deinit();

    return 0;
}

