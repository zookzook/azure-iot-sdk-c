// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.


#include "esp_common.h"
#include "wifi_config.h"

// The ESP8266 sdk shows user_rf_cal_sector_set as a required function
// that must be included in all user_main files.
uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;
        case FLASH_SIZE_64M_MAP_1024_1024:
            rf_cal_sec = 2048 - 5;
            break;
        case FLASH_SIZE_128M_MAP_1024_1024:
            rf_cal_sec = 4096 - 5;
            break;
        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

#include "esp_common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "lwip/opt.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "openssl/ssl.h"
#include "iothub_client_sample_mqtt.h"
#include "lwip/apps/sntp.h"
#include "lwip/apps/sntp_time.h"
#include "uart.h"

#define OPENSSL_DEMO_THREAD_NAME "ssl_demo"
#define OPENSSL_DEMO_THREAD_STACK_WORDS 1024*2
#define OPENSSL_DEMO_THREAD_PRORIOTY 6

static os_timer_t timer;

LOCAL void ICACHE_FLASH_ATTR mqtt_sample()
{
    while (1) {
        struct ip_info info;
        wifi_get_ip_info(STATION_IF, &info);
        if (info.ip.addr != 0) {
            break;
        }
        else {
            vTaskDelay(10);
        }
    }
    iothub_client_sample_mqtt_run();
    vTaskDelete(NULL);
}

LOCAL void ICACHE_FLASH_ATTR wait_for_connection_ready(uint8 flag)
{
    unsigned char ret = 0;
    struct ip_info ipconfig;
    xTaskHandle openssl_handle;
    os_timer_disarm(&timer);
    ret = wifi_station_get_connect_status();

    os_printf("ret %d\n", ret);

    if (ret == STATION_GOT_IP) {
        printf("azure iot program starts %d\n", system_get_free_heap_size());

        int result;
        result = xTaskCreate(mqtt_sample,
            OPENSSL_DEMO_THREAD_NAME,
            OPENSSL_DEMO_THREAD_STACK_WORDS,
            NULL,
            OPENSSL_DEMO_THREAD_PRORIOTY,
            &openssl_handle);

        if (result != pdPASS) {
            os_printf("create thread %s failed\n", OPENSSL_DEMO_THREAD_NAME);
            return;
        }

    }
    else {
        os_timer_disarm(&timer);
        os_timer_setfn(&timer, (os_timer_func_t *)wait_for_connection_ready, NULL);
        os_timer_arm(&timer, 2000, 0);
    }
}

LOCAL void ICACHE_FLASH_ATTR startWiFi()
{
    int ssidLen = strlen(WIFI_SSID);
    int passwordLen = strlen(WIFI_PASSWORD);

    struct station_config sta_conf;
    wifi_set_opmode(STATION_MODE);
    memset(sta_conf.ssid, 0, sizeof(sta_conf.ssid));
    memset(sta_conf.password, 0, sizeof(sta_conf.password));
    memset(sta_conf.bssid, 0, sizeof(sta_conf.bssid));
    memcpy(sta_conf.ssid, WIFI_SSID, ssidLen);
    memcpy(sta_conf.password, WIFI_PASSWORD, passwordLen);
    sta_conf.bssid_set = 0;
    // We don't want to save wifi params to flash each time, we boot,
    // so wifi_station_set_config_current is used instead of wifi_station_set_config.
    // This call starts the connection process.
    wifi_station_set_config_current(&sta_conf);
}

LOCAL void ICACHE_FLASH_ATTR configWiFi()
{
    startWiFi();

    os_timer_disarm(&timer);
    os_timer_setfn(&timer, (os_timer_func_t *)wait_for_connection_ready, NULL);
    os_timer_arm(&timer, 2000, 0);
}
 
/*

LOCAL void ICACHE_FLASH_ATTR start_azure_iot()
{
    // configWiFi kicks off the process of connecting to the AP when 
    // called from outside a task

}
*/

// This is the entry point for the ESP8266 to launch the user task.
void user_init(void)
{
    // The ESP8266 does not start up at a predictable baud rate,
    // so the connected terminal will show garbage for a while until
    // the uart_div_modify() takes effect. This is considered normal.
    uart_div_modify(0, UART_CLK_FREQ / 115200);
    printf("Waiting for baud change\n");
    printf("Waiting for baud change\n");
    printf("Waiting for baud change\n");
    printf("ESP8266 RTOS SDK version:%s\n", system_get_sdk_version());
    configWiFi();
}
