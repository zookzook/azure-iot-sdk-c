// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifdef __cplusplus
#include <cstdlib>
#include <cstddef>
#else
#include <stdlib.h>
#include <stddef.h>
#endif

static void* my_gballoc_malloc(size_t size)
{
    return malloc(size);
}

static void my_gballoc_free(void* ptr)
{
    free(ptr);
}

#include "testrunnerswitcher.h"
#include "umock_c.h"
#include "umocktypes_bool.h"
#include "umock_c_negative_tests.h"
#include "azure_c_shared_utility/macro_utils.h"

#define ENABLE_MOCKS
#include "azure_hub_modules/dps_transport_mqtt_common.h"
#include "azure_c_shared_utility/buffer_.h"
#include "azure_hub_modules/dps_transport.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/wsio.h"
#undef ENABLE_MOCKS

#include "azure_hub_modules/dps_transport_mqtt_ws_client.h"

#define ENABLE_MOCKS
#include "azure_c_shared_utility/gballoc.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/tlsio.h"

#include "azure_c_shared_utility/umock_c_prod.h"
MOCKABLE_FUNCTION(, void, on_transport_register_data_cb, DPS_TRANSPORT_RESULT, transport_result, BUFFER_HANDLE, iothub_key, const char*, assigned_hub, const char*, device_id, void*, user_ctx);
MOCKABLE_FUNCTION(, void, on_transport_status_cb, DPS_TRANSPORT_STATUS, transport_status, void*, user_ctx);
MOCKABLE_FUNCTION(, char*, on_transport_challenge_callback, const unsigned char*, nonce, size_t, nonce_len, const char*, key_name, void*, user_ctx);
MOCKABLE_FUNCTION(, XIO_HANDLE, on_mqtt_transport_io, const char*, fqdn, const HTTP_PROXY_IO_CONFIG*, proxy_info);
MOCKABLE_FUNCTION(, DPS_JSON_INFO*, on_transport_json_parse, const char*, json_document, void*, user_ctx);

#undef ENABLE_MOCKS

#define TEST_DPS_HANDLE (DPS_TRANSPORT_HANDLE)0x11111111
#define TEST_BUFFER_VALUE (BUFFER_HANDLE)0x11111112
#define TEST_INTERFACE_DESC (const IO_INTERFACE_DESCRIPTION*)0x11111113
#define TEST_XIO_HANDLE (XIO_HANDLE)0x11111114

static const char* TEST_URI_VALUE = "dps_uri";
static const char* TEST_SCOPE_ID_VALUE = "scope_id";
static const char* TEST_REGISTRATION_ID_VALUE = "registration_id";
static const char* TEST_DPS_API_VALUE = "dps_api";
static const char* TEST_X509_CERT_VALUE = "x509_cert";
static const char* TEST_CERT_VALUE = "certificate";
static const char* TEST_PRIVATE_KEY_VALUE = "private_key";
static const char* TEST_HOST_ADDRESS_VALUE = "host_address";

DPS_MQTT_TRANSPORT_IO g_transport_io = NULL;

static pfdps_transport_create dps_mqtt_ws_transport_create;
static pfdps_transport_destroy dps_mqtt_ws_transport_destroy;
static pfdps_transport_open dps_mqtt_ws_transport_open;
static pfdps_transport_close dps_mqtt_ws_transport_close;
static pfdps_transport_register_device dps_mqtt_ws_transport_register_device;
static pfdps_transport_get_operation_status dps_mqtt_ws_transport_get_op_status;
static pfdps_transport_dowork dps_mqtt_ws_transport_dowork;
static pfdps_transport_set_trace dps_mqtt_ws_transport_set_trace;
static pfdps_transport_set_x509_cert dps_mqtt_ws_transport_x509_cert;
static pfdps_transport_set_trusted_cert dps_mqtt_ws_transport_trusted_cert;
static pfdps_transport_set_proxy dps_mqtt_ws_transport_set_proxy;

TEST_DEFINE_ENUM_TYPE(DPS_TRANSPORT_RESULT, DPS_TRANSPORT_RESULT_VALUES);
IMPLEMENT_UMOCK_C_ENUM_TYPE(DPS_TRANSPORT_RESULT, DPS_TRANSPORT_RESULT_VALUES);

TEST_DEFINE_ENUM_TYPE(DPS_TRANSPORT_STATUS, DPS_TRANSPORT_STATUS_VALUES);
IMPLEMENT_UMOCK_C_ENUM_TYPE(DPS_TRANSPORT_STATUS, DPS_TRANSPORT_STATUS_VALUES);

TEST_DEFINE_ENUM_TYPE(DPS_HSM_TYPE, DPS_HSM_TYPE_VALUES);
IMPLEMENT_UMOCK_C_ENUM_TYPE(DPS_HSM_TYPE, DPS_HSM_TYPE_VALUES);

static DPS_TRANSPORT_HANDLE my_dps_transport_common_mqtt_create(const char* uri, DPS_HSM_TYPE type, const char* scope_id, const char* registration_id, const char* dps_api_version, DPS_MQTT_TRANSPORT_IO transport_io)
{
    (void)uri;
    (void)type;
    (void)scope_id;
    (void)registration_id;
    (void)dps_api_version;
    
    g_transport_io = transport_io;
    return TEST_DPS_HANDLE;
}

DEFINE_ENUM_STRINGS(UMOCK_C_ERROR_CODE, UMOCK_C_ERROR_CODE_VALUES)

static void on_umock_c_error(UMOCK_C_ERROR_CODE error_code)
{
    char temp_str[256];
    (void)snprintf(temp_str, sizeof(temp_str), "umock_c reported error :%s", ENUM_TO_STRING(UMOCK_C_ERROR_CODE, error_code));
    ASSERT_FAIL(temp_str);
}

static TEST_MUTEX_HANDLE g_testByTest;
static TEST_MUTEX_HANDLE g_dllByDll;

BEGIN_TEST_SUITE(dps_transport_mqtt_ws_client_ut)

    TEST_SUITE_INITIALIZE(suite_init)
    {
        TEST_INITIALIZE_MEMORY_DEBUG(g_dllByDll);
        g_testByTest = TEST_MUTEX_CREATE();
        ASSERT_IS_NOT_NULL(g_testByTest);

        (void)umock_c_init(on_umock_c_error);
        (void)umocktypes_bool_register_types();

        REGISTER_TYPE(DPS_TRANSPORT_RESULT, DPS_TRANSPORT_RESULT);
        REGISTER_TYPE(DPS_TRANSPORT_STATUS, DPS_TRANSPORT_STATUS);
        REGISTER_TYPE(DPS_HSM_TYPE, DPS_HSM_TYPE);

        REGISTER_UMOCK_ALIAS_TYPE(DPS_TRANSPORT_HANDLE, void*);
        REGISTER_UMOCK_ALIAS_TYPE(BUFFER_HANDLE, void*);
        REGISTER_UMOCK_ALIAS_TYPE(DPS_TRANSPORT_REGISTER_DATA_CALLBACK, void*);
        REGISTER_UMOCK_ALIAS_TYPE(DPS_TRANSPORT_STATUS_CALLBACK, void*);
        REGISTER_UMOCK_ALIAS_TYPE(DPS_TRANSPORT_CHALLENGE_CALLBACK, void*);
        REGISTER_UMOCK_ALIAS_TYPE(dps_mqtt_ws_transport_IO, void*);
        REGISTER_UMOCK_ALIAS_TYPE(HTTP_CLIENT_REQUEST_TYPE, int);
        REGISTER_UMOCK_ALIAS_TYPE(XIO_HANDLE, void*);
        REGISTER_UMOCK_ALIAS_TYPE(DPS_MQTT_TRANSPORT_IO, void*);
        REGISTER_UMOCK_ALIAS_TYPE(DPS_TRANSPORT_JSON_PARSE, void*);

        REGISTER_GLOBAL_MOCK_HOOK(gballoc_malloc, my_gballoc_malloc);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(gballoc_malloc, NULL);
        REGISTER_GLOBAL_MOCK_HOOK(gballoc_free, my_gballoc_free);

        REGISTER_GLOBAL_MOCK_RETURN(wsio_get_interface_description, TEST_INTERFACE_DESC);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(wsio_get_interface_description, NULL);

        REGISTER_GLOBAL_MOCK_RETURN(platform_get_default_tlsio, TEST_INTERFACE_DESC);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(platform_get_default_tlsio, NULL);
        REGISTER_GLOBAL_MOCK_RETURN(http_proxy_io_get_interface_description, TEST_INTERFACE_DESC);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(http_proxy_io_get_interface_description, NULL);
        REGISTER_GLOBAL_MOCK_RETURN(xio_create, TEST_XIO_HANDLE);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(xio_create, NULL);

        REGISTER_GLOBAL_MOCK_HOOK(dps_transport_common_mqtt_create, my_dps_transport_common_mqtt_create);
        REGISTER_GLOBAL_MOCK_RETURN(dps_transport_common_mqtt_open, 0);
        REGISTER_GLOBAL_MOCK_RETURN(dps_transport_common_mqtt_close, 0);
        REGISTER_GLOBAL_MOCK_RETURN(dps_transport_common_mqtt_register_device, 0);
        REGISTER_GLOBAL_MOCK_RETURN(dps_transport_common_mqtt_get_operation_status, 0);
        REGISTER_GLOBAL_MOCK_RETURN(dps_transport_common_mqtt_set_trace, 0);
        REGISTER_GLOBAL_MOCK_RETURN(dps_transport_common_mqtt_x509_cert, 0);
        REGISTER_GLOBAL_MOCK_RETURN(dps_transport_common_mqtt_set_trusted_cert, 0);
        REGISTER_GLOBAL_MOCK_RETURN(dps_transport_common_mqtt_set_proxy, 0);

        dps_mqtt_ws_transport_create = DPS_MQTT_WS_Protocol()->dps_transport_create;
        dps_mqtt_ws_transport_destroy = DPS_MQTT_WS_Protocol()->dps_transport_destroy;
        dps_mqtt_ws_transport_open = DPS_MQTT_WS_Protocol()->dps_transport_open;
        dps_mqtt_ws_transport_close = DPS_MQTT_WS_Protocol()->dps_transport_close;
        dps_mqtt_ws_transport_register_device = DPS_MQTT_WS_Protocol()->dps_transport_register;
        dps_mqtt_ws_transport_get_op_status = DPS_MQTT_WS_Protocol()->dps_transport_get_op_status;
        dps_mqtt_ws_transport_dowork = DPS_MQTT_WS_Protocol()->dps_transport_dowork;
        dps_mqtt_ws_transport_set_trace = DPS_MQTT_WS_Protocol()->dps_transport_set_trace;
        dps_mqtt_ws_transport_x509_cert = DPS_MQTT_WS_Protocol()->dps_transport_x509_cert;
        dps_mqtt_ws_transport_trusted_cert = DPS_MQTT_WS_Protocol()->dps_transport_trusted_cert;
        dps_mqtt_ws_transport_set_proxy = DPS_MQTT_WS_Protocol()->dps_transport_set_proxy;
    }

    TEST_SUITE_CLEANUP(suite_cleanup)
    {
        umock_c_deinit();

        TEST_MUTEX_DESTROY(g_testByTest);
        TEST_DEINITIALIZE_MEMORY_DEBUG(g_dllByDll);
    }

    TEST_FUNCTION_INITIALIZE(method_init)
    {
        if (TEST_MUTEX_ACQUIRE(g_testByTest))
        {
            ASSERT_FAIL("Could not acquire test serialization mutex.");
        }
        umock_c_reset_all_calls();
        g_transport_io = NULL;
    }

    TEST_FUNCTION_CLEANUP(method_cleanup)
    {
        TEST_MUTEX_RELEASE(g_testByTest);
    }

    /* Tests_DPS_TRANSPORT_MQTT_WS_CLIENT_07_001: [ dps_transport_mqtt_ws_create shall call the dps_transport_common_mqtt_create function with mqtt_transport_io transport IO estabishment. ] */
    TEST_FUNCTION(dps_transport_mqtt_ws_create_succeed)
    {
        //arrange
        STRICT_EXPECTED_CALL(dps_transport_common_mqtt_create(TEST_URI_VALUE, DPS_HSM_TYPE_TPM, TEST_SCOPE_ID_VALUE, TEST_REGISTRATION_ID_VALUE, TEST_DPS_API_VALUE, IGNORED_PTR_ARG));

        //act
        DPS_TRANSPORT_HANDLE handle = dps_mqtt_ws_transport_create(TEST_URI_VALUE, DPS_HSM_TYPE_TPM, TEST_SCOPE_ID_VALUE, TEST_REGISTRATION_ID_VALUE, TEST_DPS_API_VALUE);

        //assert
        ASSERT_IS_NOT_NULL(handle);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_DPS_TRANSPORT_MQTT_WS_CLIENT_07_014: [ On success mqtt_transport_io shall return an allocated DPS_TRANSPORT_IO_INFO structure. ] */
    TEST_FUNCTION(mqtt_transport_ws_io_succeed)
    {
        XIO_HANDLE dps_io_info;
        (void)dps_mqtt_ws_transport_create(TEST_URI_VALUE, DPS_HSM_TYPE_TPM, TEST_SCOPE_ID_VALUE, TEST_REGISTRATION_ID_VALUE, TEST_DPS_API_VALUE);
        umock_c_reset_all_calls();

        //arrange
        STRICT_EXPECTED_CALL(wsio_get_interface_description());
        STRICT_EXPECTED_CALL(platform_get_default_tlsio());
        STRICT_EXPECTED_CALL(xio_create(TEST_INTERFACE_DESC, IGNORED_PTR_ARG));
        STRICT_EXPECTED_CALL(xio_setoption(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_PTR_ARG));

        //act
        dps_io_info = g_transport_io(TEST_URI_VALUE, NULL);

        //assert
        ASSERT_IS_NOT_NULL(dps_io_info);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_DPS_TRANSPORT_MQTT_WS_CLIENT_07_014: [ On success mqtt_transport_io shall return an allocated DPS_TRANSPORT_IO_INFO structure. ] */
    TEST_FUNCTION(mqtt_transport_ws_io_x509_succeed)
    {
        XIO_HANDLE dps_io_info;
        (void)dps_mqtt_ws_transport_create(TEST_URI_VALUE, DPS_HSM_TYPE_X509, TEST_SCOPE_ID_VALUE, TEST_REGISTRATION_ID_VALUE, TEST_DPS_API_VALUE);
        umock_c_reset_all_calls();

        //arrange
        STRICT_EXPECTED_CALL(wsio_get_interface_description());
        STRICT_EXPECTED_CALL(platform_get_default_tlsio());
        STRICT_EXPECTED_CALL(xio_create(TEST_INTERFACE_DESC, IGNORED_PTR_ARG));
        STRICT_EXPECTED_CALL(xio_setoption(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_PTR_ARG));

        //act
        dps_io_info = g_transport_io(TEST_URI_VALUE, NULL);

        //assert
        ASSERT_IS_NOT_NULL(dps_io_info);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_DPS_TRANSPORT_MQTT_WS_CLIENT_07_013: [ If any failure is encountered mqtt_transport_io shall return NULL ] */
    TEST_FUNCTION(mqtt_transport_ws_io_fail)
    {
        XIO_HANDLE dps_io_info;
        (void)dps_mqtt_ws_transport_create(TEST_URI_VALUE, DPS_HSM_TYPE_TPM, TEST_SCOPE_ID_VALUE, TEST_REGISTRATION_ID_VALUE, TEST_DPS_API_VALUE);
        umock_c_reset_all_calls();

        int negativeTestsInitResult = umock_c_negative_tests_init();
        ASSERT_ARE_EQUAL(int, 0, negativeTestsInitResult);

        umock_c_reset_all_calls();

        //arrange
        STRICT_EXPECTED_CALL(wsio_get_interface_description());
        STRICT_EXPECTED_CALL(platform_get_default_tlsio());
        STRICT_EXPECTED_CALL(xio_create(TEST_INTERFACE_DESC, IGNORED_PTR_ARG));

        umock_c_negative_tests_snapshot();

        size_t count = umock_c_negative_tests_call_count();
        for (size_t index = 0; index < count; index++)
        {
            umock_c_negative_tests_reset();
            umock_c_negative_tests_fail_call(index);

            char tmp_msg[64];
            sprintf(tmp_msg, "g_transport_io failure in test %zu/%zu", index, count);

            //act
            dps_io_info = g_transport_io(TEST_URI_VALUE, NULL);

            //assert 
            ASSERT_IS_NULL_WITH_MSG(dps_io_info, tmp_msg);
        }

        //cleanup
        umock_c_negative_tests_deinit();
    }

    /* Tests_DPS_TRANSPORT_MQTT_WS_CLIENT_07_012: [ If proxy_info is not NULL, mqtt_transport_io shall construct a HTTP_PROXY_IO_CONFIG object and assign it to TLSIO_CONFIG underlying_io_parameters ] */
    TEST_FUNCTION(mqtt_transport_ws_io_w_http_proxy_succeed)
    {
        HTTP_PROXY_OPTIONS proxy_info;
        XIO_HANDLE dps_io_info;
        (void)dps_mqtt_ws_transport_create(TEST_URI_VALUE, DPS_HSM_TYPE_TPM, TEST_SCOPE_ID_VALUE, TEST_REGISTRATION_ID_VALUE, TEST_DPS_API_VALUE);
        umock_c_reset_all_calls();

        //arrange
        STRICT_EXPECTED_CALL(wsio_get_interface_description());
        STRICT_EXPECTED_CALL(platform_get_default_tlsio());
        STRICT_EXPECTED_CALL(http_proxy_io_get_interface_description());
        STRICT_EXPECTED_CALL(xio_create(TEST_INTERFACE_DESC, IGNORED_PTR_ARG));
        STRICT_EXPECTED_CALL(xio_setoption(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_PTR_ARG));
        proxy_info.host_address = TEST_HOST_ADDRESS_VALUE;
        proxy_info.username = TEST_PRIVATE_KEY_VALUE;
        proxy_info.password = TEST_HOST_ADDRESS_VALUE;
        proxy_info.port = 2225;

        //act
        dps_io_info = g_transport_io(TEST_URI_VALUE, &proxy_info);

        //assert
        ASSERT_IS_NOT_NULL(dps_io_info);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_DPS_TRANSPORT_MQTT_WS_CLIENT_07_002: [ dps_transport_mqtt_ws_destroy shall invoke the dps_transport_common_mqtt_destroy method ] */
    TEST_FUNCTION(dps_transport_mqtt_ws_destroy_succeed)
    {
        //arrange
        STRICT_EXPECTED_CALL(dps_transport_common_mqtt_destroy(TEST_DPS_HANDLE));

        //act
        dps_mqtt_ws_transport_destroy(TEST_DPS_HANDLE);

        //assert
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_DPS_TRANSPORT_MQTT_WS_CLIENT_07_003: [ dps_transport_mqtt_ws_open shall invoke the dps_transport_common_mqtt_open method ] */
    TEST_FUNCTION(dps_transport_mqtt_ws_open_succeed)
    {
        //arrange
        STRICT_EXPECTED_CALL(dps_transport_common_mqtt_open(TEST_DPS_HANDLE, TEST_BUFFER_VALUE, TEST_BUFFER_VALUE, on_transport_register_data_cb, NULL, on_transport_status_cb, NULL));

        //act
        int result = dps_mqtt_ws_transport_open(TEST_DPS_HANDLE, TEST_BUFFER_VALUE, TEST_BUFFER_VALUE, on_transport_register_data_cb, NULL, on_transport_status_cb, NULL);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_DPS_TRANSPORT_MQTT_WS_CLIENT_07_004: [ dps_transport_mqtt_ws_close shall invoke the dps_transport_common_mqtt_close method ] */
    TEST_FUNCTION(dps_transport_mqtt_ws_close_succeed)
    {
        //arrange
        STRICT_EXPECTED_CALL(dps_transport_common_mqtt_close(TEST_DPS_HANDLE));

        //act
        int result = dps_mqtt_ws_transport_close(TEST_DPS_HANDLE);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_DPS_TRANSPORT_MQTT_WS_CLIENT_07_005: [ dps_transport_mqtt_ws_register_device shall invoke the dps_transport_common_mqtt_register_device method ] */
    TEST_FUNCTION(dps_transport_mqtt_ws_register_device_succeed)
    {
        //arrange
        STRICT_EXPECTED_CALL(dps_transport_common_mqtt_register_device(TEST_DPS_HANDLE, on_transport_challenge_callback, NULL, on_transport_json_parse, NULL));

        //act
        int result = dps_mqtt_ws_transport_register_device(TEST_DPS_HANDLE, on_transport_challenge_callback, NULL, on_transport_json_parse, NULL);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_DPS_TRANSPORT_MQTT_WS_CLIENT_07_006: [ dps_transport_mqtt_ws_get_operation_status shall invoke the dps_transport_common_mqtt_get_operation_status method ] */
    TEST_FUNCTION(dps_transport_mqtt_ws_get_operation_status_succeed)
    {
        //arrange
        STRICT_EXPECTED_CALL(dps_transport_common_mqtt_get_operation_status(TEST_DPS_HANDLE));

        //act
        int result = dps_mqtt_ws_transport_get_op_status(TEST_DPS_HANDLE);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_DPS_TRANSPORT_MQTT_WS_CLIENT_07_007: [ dps_transport_mqtt_ws_dowork shall invoke the dps_transport_common_mqtt_dowork method ] */
    TEST_FUNCTION(dps_transport_mqtt_ws_dowork_succeed)
    {
        //arrange
        STRICT_EXPECTED_CALL(dps_transport_common_mqtt_dowork(TEST_DPS_HANDLE));

        //act
        dps_mqtt_ws_transport_dowork(TEST_DPS_HANDLE);

        //assert
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_DPS_TRANSPORT_MQTT_WS_CLIENT_07_008: [ dps_transport_mqtt_ws_set_trace shall invoke the dps_transport_common_mqtt_set_trace method ] */
    TEST_FUNCTION(dps_transport_mqtt_ws_set_trace_succeed)
    {
        //arrange
        STRICT_EXPECTED_CALL(dps_transport_common_mqtt_set_trace(TEST_DPS_HANDLE, true));

        //act
        int result = dps_mqtt_ws_transport_set_trace(TEST_DPS_HANDLE, true);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_DPS_TRANSPORT_MQTT_WS_CLIENT_07_009: [ dps_transport_mqtt_ws_x509_cert shall invoke the dps_trans_common_mqtt_x509_cert method ] */
    TEST_FUNCTION(dps_transport_mqtt_ws_x509_cert_succeed)
    {
        //arrange
        STRICT_EXPECTED_CALL(dps_transport_common_mqtt_x509_cert(TEST_DPS_HANDLE, TEST_X509_CERT_VALUE, TEST_PRIVATE_KEY_VALUE));

        //act
        int result = dps_mqtt_ws_transport_x509_cert(TEST_DPS_HANDLE, TEST_X509_CERT_VALUE, TEST_PRIVATE_KEY_VALUE);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_DPS_TRANSPORT_MQTT_WS_CLIENT_07_010: [ dps_transport_mqtt_ws_set_trusted_cert shall invoke the dps_transport_common_mqtt_set_trusted_cert method ] */
    TEST_FUNCTION(dps_transport_mqtt_ws_set_trusted_cert_succeed)
    {
        //arrange
        STRICT_EXPECTED_CALL(dps_transport_common_mqtt_set_trusted_cert(TEST_DPS_HANDLE, TEST_CERT_VALUE));

        //act
        int result = dps_mqtt_ws_transport_trusted_cert(TEST_DPS_HANDLE, TEST_CERT_VALUE);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_DPS_TRANSPORT_MQTT_WS_CLIENT_07_011: [ dps_transport_mqtt_ws_set_proxy shall invoke the dps_transport_common_mqtt_set_proxy method ] */
    TEST_FUNCTION(dps_transport_mqtt_ws_set_proxy_succeed)
    {
        //arrange
        HTTP_PROXY_OPTIONS proxy_options = { 0 };
        proxy_options.host_address = TEST_HOST_ADDRESS_VALUE;
        proxy_options.port = 443;

        STRICT_EXPECTED_CALL(dps_transport_common_mqtt_set_proxy(TEST_DPS_HANDLE, &proxy_options));

        //act
        int result = dps_mqtt_ws_transport_set_proxy(TEST_DPS_HANDLE, &proxy_options);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    END_TEST_SUITE(dps_transport_mqtt_ws_client_ut)
