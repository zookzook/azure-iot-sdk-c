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

DECLARE_DEVICETWIN_MODEL(Car,
    WITH_REPORTED_PROPERTY(ascii_char_ptr, name),
    WITH_REPORTED_PROPERTY(uint8_t, wf_state), /* 0 = available, 1 = hailed, 2 = in service, 3 = dropping off */
    WITH_REPORTED_PROPERTY(Geo, location),
    WITH_REPORTED_PROPERTY(Geo, destination),

    WITH_DESIRED_PROPERTY(Geo, fare_location, onRideHailed)
);

END_NAMESPACE(Whober);

static void reportedStateCallback(int status_code, void* userContextCallback)
{
    (void)(userContextCallback);
    (void)printf("IoTHub: reported properties delivered with status_code = %u\n", status_code);
}

static void desiredStateCallback(DEVICE_TWIN_UPDATE_STATE update_state, const unsigned char* payLoad, size_t size, void* userContextCallback)
{
    (void)(userContextCallback);
    (void)printf("\n\tdesiredStateCallback(%d): %.*s\n\n", update_state, (int)size, (const char*)payLoad);
}

/* *** BUG *** */
/* This function never gets called when I call `send desired properties` from device explorer. */
void onRideHailed(void* argument)
{
    Car *car = (Car *)argument;
    (void)printf("\n\t%s (%.2f, %.2f)\n\n", car->name, car->fare_location.latitude, car->fare_location.longitude);
    if (car->wf_state == 0)
    {
        car->wf_state = 1;
    }
    else
    {
        (void)printf("onDesired is called out of order!!!");
    }
}

static bool Move(Geo *cur, Geo *dest)
{
    bool la_moved;
    if (cur->latitude < dest->latitude)
    {
        cur->latitude++;
        la_moved = true;
    }
    else if (cur->latitude > dest->latitude)
    {
        cur->latitude--;
        la_moved = true;
    }
    else
    {
        la_moved = false;
    }

    bool lg_moved;
    if (cur->longitude < dest->longitude)
    {
        cur->longitude++;
        lg_moved = true;
    }
    else if (cur->longitude > dest->longitude)
    {
        cur->longitude--;
        lg_moved = true;
    }
    else
    {
        lg_moved = false;
    }

    return (la_moved || lg_moved);
}

static void UpdateCarLocation(Car *car)
{
    switch (car->wf_state)
    {
        case 1 /* hailed - on my way to pick a fair */:
            if (Move(&(car->location), &(car->fare_location)) == false)
            {
                car->wf_state = 2;
            }
            break;

        case 2 /* in service - delivering a fair */:
            if (Move(&(car->location), &(car->destination)) == false)
            {
                car->wf_state = 3;
            }
            break;

        case 3 /* in service - delivering a fair */:
            car->wf_state = 0;
            break;

        default:
            break;
    }
}

DEFINE_ENUM_STRINGS(IOTHUB_CLIENT_CONNECTION_STATUS, IOTHUB_CLIENT_CONNECTION_STATUS_REASON_VALUES);
DEFINE_ENUM_STRINGS(IOTHUB_CLIENT_CONNECTION_STATUS_REASON, IOTHUB_CLIENT_CONNECTION_STATUS_REASON_VALUES);
static void connCB(IOTHUB_CLIENT_CONNECTION_STATUS result, IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason, void* userContextCallback)
{
    (void)userContextCallback;
    printf("\t[%s : %s]\n", ENUM_TO_STRING(IOTHUB_CLIENT_CONNECTION_STATUS, result), ENUM_TO_STRING(IOTHUB_CLIENT_CONNECTION_STATUS_REASON, reason));
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

            /* create an IoTHub client */
            IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(cs, MQTT_Protocol);
            if (iotHubClientHandle == NULL)
            {
                (void)printf("Failure creating IoTHubClient handle");
                return -2;
            }

            (void)IoTHubClient_LL_SetConnectionStatusCallback(iotHubClientHandle, connCB, NULL);

            /* *** BUG *** */
            /* I am expecting the `desiredStateCallback` to be called when the service makes a 'desired' properties call.
             * Currently, the callback never happens - I am using the DeviceExplorer to test the code.
            */
            if (IoTHubClient_LL_SetDeviceTwinCallback(iotHubClientHandle, desiredStateCallback, NULL) == IOTHUB_CLIENT_OK)
            {
                // Turn on Log 
                bool trace = true;
                (void)IoTHubClient_LL_SetOption(iotHubClientHandle, "logtrace", &trace);

                Car* car = IoTHubDeviceTwin_LL_CreateCar(iotHubClientHandle);
                if (car == NULL)
                {
                    printf("Failure in IoTHubDeviceTwin_CreateCar");
                }
                else
                {
                    /*setting values for reported properties*/
                    car->name = "desire";
                    car->wf_state = 0;

                    /* *** BUG *** */
                    /* The call to _SendReportedStateCar will cause a memory error if I forget to initialize any of the following four properties.
                    */
                    car->destination.latitude = 42;
                    car->destination.longitude = 40;
                    car->location.latitude = 42;
                    car->location.longitude = 40;

                    int ii = 0;
                    do
                    {
                        IoTHubClient_LL_DoWork(iotHubClientHandle);

                        /*sending the values to IoTHub*/
                        UpdateCarLocation(car);
                        if (ii < 5)
                        {
                            (void)IoTHubDeviceTwin_LL_SendReportedStateCar(car, reportedStateCallback, car);
                            ii = 0;
                        }
                        else
                        {
                            ii++;
                        }
                        ThreadAPI_Sleep(1000);
                        (void)printf("\n***\n");
                    } while (true);
                }
                IoTHubDeviceTwin_DestroyCar(car);
            }
            else
            {
                (void)printf("failed to set device twin callback");
            }
            IoTHubClient_LL_Destroy(iotHubClientHandle);
        }
    }
    platform_deinit();

    return 0;
}

