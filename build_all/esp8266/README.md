# ESP8266 Gate Build

This project exercises the ESP32 cross-compiler against the Azure IoT C SDK. This project is Linux-only, and is built by the `esp8266_c.sh` script in this SDK's Jenkins directory.

### Project script

The script for the ESP8266 gate build is in the C SDK at `jenkins/esp8266_c.sh`

### Project environment
The ESP8266 build tools expect two pieces of environment information
* **ESP8266_RTOS_SDK** - this export must be set to the directory containing the ESP RTOS SDK
* **ESP8266_TOOLS** - this export must be set to the ESP8266 toolchain built using *esp-open-sdk* as described below.

### Project setup

Here are the instructions for setting up the Linux environment for the ESP8266 cross-compiler:


##### 1 - Install prerequisites:  
`$ sudo apt-get install make unrar-free autoconf automake libtool gcc g++ gperf \` <br/>
 ` flex bison texinfo gawk ncurses-dev libexpat-dev python-dev python python-serial \`<br/>
    `sed git unzip bash help2man wget bzip2`

then

`sudo apt-get install libtool-bin`

##### 2 - Install the ESP8266 toolchain:

Clone the ESP Open SDK project:

`cd workspace`<br/>
`git clone  --recursive https://github.com/pfalcon/esp-open-sdk.git`

Build the tools:

`cd esp-open-sdk`<br/>
`make STANDALONE=n`

The `make` process takes about 7 minutes on a dedicated Ubuntu box.

##### 3 - Create an export for the toolchain location:<br/>
`export ESP8266_TOOLS="whatever/workspace/esp-open-sdk"`

##### 4 - Install the ESP8266 RTOS SDK:

`cd workspace`<br/>
`git clone https://github.com/espressif/ESP8266_RTOS_SDK.git`

##### 5 - Export the ESP32 SDK location:

`export ESP8266_RTOS_SDK="/whatever/workspace/ESP8266_RTOS_SDK"`

##### 6 - Azure C SDK location:
The above steps need only be done once. When cloning the Azure IoT C SDK, please put it in a directory named
`workspace/ESP8266:

`git clone --recursive https://github.com/Azure/azure-iot-sdk-c.git ESP8266`
