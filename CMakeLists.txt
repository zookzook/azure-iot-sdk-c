#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

cmake_minimum_required(VERSION 2.8.11)
project(azure_iot_sdks)

set(IOT_SDK_VERSION 1.1.24)

# Include the common build rules for the C SDK
include(configs/azure_iot_build_rules.cmake)

set(dps_auth_type "" CACHE STRING "The type of authentication used with the DPS client")
set(use_dps_client OFF CACHE BOOL "Enable DPS client")
set(dps_hsm_x509 OFF CACHE BOOL "x509 type of hsm used with the DPS client")
set(dps_hsm_sastoken OFF CACHE BOOL "tpm type of hsm used with the DPS client")
set(dps_custom_hsm OFF CACHE BOOL "hsm type of custom used with the DPS client")
set(dps_hsm_tpm_simulator OFF CACHE BOOL "tpm simulator type of hsm used with the DPS client")
set(dps_hsm_custom_repo "" CACHE STRING "Custom repo")
set(dps_hsm_custom_lib "" CACHE STRING "Custom repo library")
set(DPS_PROTOCOL_FEATURE OFF CACHE BOOL "Disable DPS protocols")

#the following variables are project-wide and can be used with cmake-gui
option(use_amqp "set use_amqp to ON if amqp is to be used, set to OFF to not use amqp" ON)
option(use_http "set use_http to ON if http is to be used, set to OFF to not use http" ON)
option(use_mqtt "set use_mqtt to ON if mqtt is to be used, set to OFF to not use mqtt" ON)
option(run_e2e_tests "set run_e2e_tests to ON to run e2e tests (default is OFF)" OFF)
option(run_unittests "set run_unittests to ON to run unittests (default is OFF)" OFF)
option(run_longhaul_tests "set run_longhaul_tests to ON to run longhaul tests (default is OFF)[if possible, they are always build]" OFF)
option(skip_samples "set skip_samples to ON to skip building samples (default is OFF)[if possible, they are always build]" OFF)
option(build_service_client "controls whether the iothub_service_client is built or not" ON)
option(build_python "builds the Python native iothub_client module" OFF)
option(build_javawrapper "builds the native iothub_client library for java C wrapper" OFF)
option(dont_use_uploadtoblob "set dont_use_uploadtoblob to ON if the functionality of upload to blob is to be excluded, OFF otherwise. It requires HTTP" OFF)
option(no_logging "disable logging" OFF)
option(use_installed_dependencies "set use_installed_dependencies to ON to use installed packages instead of building dependencies from submodules" OFF)
option(use_firmware_update "build the Raspberry PI firmware_update sample" OFF)
option(build_as_dynamic "build the IoT SDK libaries as dynamic"  OFF)
option(build_network_e2e "build network E2E tests" OFF)

if(WIN32)
    option(use_openssl "set use_openssl to ON to use OpenSSL." OFF)
else()
    option(use_openssl "set use_openssl to ON to use OpenSSL." ON)
endif()

#openssl samples on Windows need to have a trusted cert set
if(WIN32 AND ${use_openssl})
    option(set_trusted_cert_in_samples "set set_trusted_cert_in_samples to ON to have the samples set the SDK's built-in CA as TrustedCerts" ON)
else()
    option(set_trusted_cert_in_samples "set set_trusted_cert_in_samples to ON to have the samples set the SDK's built-in CA as TrustedCerts" OFF)
endif()

if ("${dps_auth_type}" STREQUAL "tpm")
    set(dps_hsm_x509 OFF)
    set(dps_hsm_sastoken ON)
    set(dps_hsm_tpm_simulator OFF)
    set(use_dps_client ON)
    set(dps_custom_hsm OFF)
elseif("${dps_auth_type}" STREQUAL "tpm_simulator")
    set(dps_hsm_x509 OFF)
    set(dps_hsm_sastoken ON)
    set(dps_hsm_tpm_simulator ON)
    set(use_dps_client ON)
    set(dps_custom_hsm OFF)
elseif("${dps_auth_type}" STREQUAL "x509")
    set(dps_hsm_sastoken OFF)
    set(dps_hsm_tpm_simulator OFF)
    set(dps_hsm_x509 ON)
    set(use_dps_client ON)
    set(dps_custom_hsm OFF)
elseif(${dps_auth_type})
    STRING(LENGTH ${dps_auth_type} hsm_length)
    if (hsm_length GREATER 0)
        message(FATAL_ERROR "Unknown DPS HSM value specified: ${dps_auth_type}")
    endif()
endif()

#setting nuget_e2e_tests will only generate e2e tests to run with nuget packages.  Install-packages from Package Manager Console in VS before building the projects
option(nuget_e2e_tests "set nuget_e2e_tests to ON to generate e2e tests to run with nuget packages (default is OFF)" OFF)

#check for conflicting options
if(NOT ${use_http})
    MESSAGE( "Setting dont_use_uploadtoblob to ON because use_http is OFF")
    set(dont_use_uploadtoblob "ON")
    MESSAGE( STATUS "use_http:         " ${use_http} )
    MESSAGE( STATUS "dont_use_uploadtoblob:         " ${dont_use_uploadtoblob} )
endif()

if(${dont_use_uploadtoblob})
    add_definitions(-DDONT_USE_UPLOADTOBLOB)
endif()

if(${no_logging})
    add_definitions(-DNO_LOGGING)
endif()

#Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(IN_OPENWRT)
    ADD_DEFINITIONS("$ENV{TARGET_LDFLAGS}" "$ENV{TARGET_CPPFLAGS}" "$ENV{TARGET_CFLAGS}")
    INCLUDE_DIRECTORIES("$ENV{TOOLCHAIN_DIR}/usr/include" "$ENV{TARGET_LDFLAGS}" "$ENV{TARGET_CPPFLAGS}" "$ENV{TARGET_CFLAGS}")
endif()

include("dependencies.cmake")

set_platform_files(${CMAKE_CURRENT_LIST_DIR}/c-utility)

if(${run_unittests} OR ${run_e2e_tests})
    include("dependencies-test.cmake")
endif()


include("configs/azure_iot_sdksFunctions.cmake")

if(WIN32)
    set(LOCK_C_FILE ${SHARED_UTIL_ADAPTER_FOLDER}/lock_win32.c)
    set(THREAD_C_FILE ${SHARED_UTIL_ADAPTER_FOLDER}/threadapi_c11.c)
else()
    set(LOCK_C_FILE ${SHARED_UTIL_ADAPTER_FOLDER}/lock_pthreads.c)
    set(THREAD_C_FILE ${SHARED_UTIL_ADAPTER_FOLDER}/threadapi_pthreads.c)
endif()

#Set CMAKE_INSTALL_LIBDIR if not defined
include(GNUInstallDirs)

if(NOT ${use_amqp} OR NOT ${use_http})
    set (build_service_client OFF)
    message(STATUS "iothub_service_client build is disabled (AMQP and HTTP support are required)")
endif()

if (${use_dps_client})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_DPS_MODULE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_DPS_MODULE")

    add_subdirectory(dps_client)
endif()

if(${build_service_client})
    add_subdirectory(iothub_service_client)
endif()

if(${run_e2e_tests} OR ${run_longhaul_tests} OR ${nuget_e2e_tests})
    add_subdirectory(testtools)
endif()

add_subdirectory(iothub_client)
add_subdirectory(serializer)

#add the device_auth sample subdirectory
if(${use_dps_client} AND NOT ${skip_samples})
    add_subdirectory(dps_client/samples)
endif()

if(NOT "${build_python}" STREQUAL "OFF")
    add_subdirectory(../device/iothub_client_python python)
    if(${build_service_client})
        add_subdirectory(../service python_service_client)
    endif()
endif()

if("${build_javawrapper}" STREQUAL "ON")
     add_subdirectory(../device/iothub_client_javawrapper javawrapper)
endif()



if(${use_installed_dependencies})
    #Install azure_iot_sdks
    set(package_location "cmake")

    include(CMakePackageConfigHelpers)

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${IOT_SDK_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    configure_file("configs/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake"
        COPYONLY
    )

    install(EXPORT azure_iot_sdksTargets
        FILE
            "${PROJECT_NAME}Targets.cmake"
        DESTINATION
            ${package_location}
    )

    install(
        FILES
            "configs/${PROJECT_NAME}Config.cmake"
            "configs/${PROJECT_NAME}Functions.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION
            ${package_location}
    )
endif()

if (${build_network_e2e})
    add_subdirectory(network_e2e/tests)
endif()
