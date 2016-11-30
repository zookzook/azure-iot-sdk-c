// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

/** @file secure_storage.h
*	@brief Provides an interface for .
*
*	@details IoTHubClient is a module that extends the IoTHubCLient_LL
*			 module with 2 features:
*				- scheduling the work for the IoTHubCLient from a
*				  thread, so that the user does not need to create their
*				  own thread
*				- thread-safe APIs
*/

#ifndef DEVICE_AUTHENTICATOR_H
#define DEVICE_AUTHENTICATOR_H

//#include "c-utility/inc/azure_c_shared_utility/macro_utils.h"
//#include "c-utility/inc/azure_c_shared_utility/strings.h"

#include "C:\repos\azure-iot-sdk-c\c-utility\inc\azure_c_shared_utility\macro_utils.h"
#include "C:\repos\azure-iot-sdk-c\c-utility\inc\azure_c_shared_utility\strings.h"


#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct DEVICE_AUTHENTICATOR_INSTANCE_TAG* DEVICE_AUTHENTICATOR_HANDLE;

    #define AUTHENTICATION_TYPE_VALUES \
        AUTHENTICATION_SAS_TOKEN, \
        AUTHENTICATION_X509

    DEFINE_ENUM(AUTHENTICATION_TYPE, AUTHENTICATION_TYPE_VALUES);

    typedef struct AUTHENTICATION_INSTANCE_TAG* AUTHENTICATION_HANDLE;

    typedef struct AUTHENTICATION_INSTANCE_TAG
    {
        AUTHENTICATION_TYPE authenticationType;
        const char* sasToken;

    } AUTHENTICATION_INSTANCE;

    #define DEVICE_AUTHENTICATOR_RESULT_VALUES \
        SECURE_STORAGE_OK, \
        SECURE_STORAGE_ERROR

    // Missing how to instantiate the TPM code - See xio (From Dan)

    DEFINE_ENUM(DEVICE_AUTHENTICATOR_RESULT, DEVICE_AUTHENTICATOR_RESULT_VALUES);

    MOCKABLE_FUNCTION(, DEVICE_AUTHENTICATOR_HANDLE, DeviceAuthenticator_Create, const char*, deviceId);

    MOCKABLE_FUNCTION(, void, DeviceAuthenticator_Destroy, DEVICE_AUTHENTICATOR_HANDLE, deviceAuthenticatorHandle);

    MOCKABLE_FUNCTION(, AUTHENTICATION_TYPE, DeviceAuthenticator_GetAuthenticationType, DEVICE_AUTHENTICATOR_HANDLE, deviceAuthenticatorHandle);

    MOCKABLE_FUNCTION(, STRING_HANDLE, DeviceAuthenticator_GenerateSASToken, DEVICE_AUTHENTICATOR_HANDLE, deviceAuthenticatorHandle, size_t, expiry);

    //MOCKABLE_FUNCTION(, STRING_HANDLE, DeviceAuthenticator_GetX509PrivateKey, DEVICE_AUTHENTICATOR_HANDLE, deviceAuthenticatorHandle);
    // TODO: No X509 for now - 

#ifdef __cplusplus
}
#endif

#endif /* SECURE_STORAGE_H */
