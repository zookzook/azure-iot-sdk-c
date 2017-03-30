#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

#this is CMakeLists.txt for devicetwin_simplesample

compileAsC99()

if(NOT ${use_mqtt})
    message(FATAL_ERROR "devicetwin_simplesample being generated without mqtt support")
endif()

set(devicetwin_simplesample_c_files
    whober_car.c
)

set(devicetwin_simplesample_h_files
	whober_car.h
)

IF(WIN32)
	#windows needs this define
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ENDIF(WIN32)

include_directories(. ${SERIALIZER_INC_FOLDER} ${SHARED_UTIL_INC_FOLDER} ${IOTHUB_CLIENT_INC_FOLDER})

add_executable(whober_car ${devicetwin_simplesample_c_files} ${devicetwin_simplesample_h_files})
target_link_libraries(whober_car iothub_client_mqtt_transport serializer iothub_client)

linkSharedUtil(whober_car)



