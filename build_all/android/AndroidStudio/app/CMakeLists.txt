# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

# build native_app_glue as a static lib
add_library(app-glue STATIC
            ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

include_directories(../../../../iothub_client/inc)
include_directories(../../../../c-utility/inc)

add_subdirectory(../../../../. .output)

set(CMAKE_LIBRARY_ARCHITECTURE ${ANDROID_ABI})

set (OPENSSL_CRYPTO_LIBRARY ${OPENSSL_LIB_DIR}/${ANDROID_ABI}/libcrypto.a)
set (OPENSSL_SSL_LIBRARY ${OPENSSL_LIB_DIR}/${ANDROID_ABI}/libssl.a)
set (OPENSSL_LIBRARIES ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})
set(CURL_LIBRARIES ${CURL_LIB_DIR}/${ANDROID_ABI}/libcurl.a)

include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${CURL_INCLUDE_DIRS})

#set(OPENSSL_ROOT_DIR ../../../../curl-android-ios/opensslroot)

#set(OPENSSL_ROOT_DIR ~/projects/Microsoft/curl-android-ios/opensslroot)
#find_package(OpenSSL REQUIRED NO_CMAKE_FIND_ROOT_PATH)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.
             src/main/cpp/native-lib.cpp src/main/cpp/sample_mqtt.c)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because system libraries are included in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )
find_library( # Sets the name of the path variable.
              z-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              z )

target_include_directories(native-lib PRIVATE
                           ${ANDROID_NDK}/sources/android/native_app_glue)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib
                       android
                       app-glue
                       iothub_client iothub_client_mqtt_transport serializer umqtt aziotsharedutil
                       ${CURL_LIBRARIES}
                       ${OPENSSL_LIBRARIES}
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} ${z-lib} )
