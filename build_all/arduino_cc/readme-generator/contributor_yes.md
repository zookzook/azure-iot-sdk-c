
## Contributors

You are in the right place to contribute Arduino-specific changes to the Azure C SDK.

### Please consider all Arduino libraries for your contribution

There are several Arduino-specific library sections, and if you make a change in an Arduino-specific file such as `travis.yml` or a `.ino` file, it would likely help others if you add your contribution to the corresponding file in all of the locations:

*  [azure-iot-sdk-c\build_all\arduino_cc\base-libraries\AzureIoTHub](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/AzureIoTHub)
*  [azure-iot-sdk-c\build_all\arduino_cc\base-libraries\AzureIoTProtocol_HTTP](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/AzureIoTProtocol_HTTP)
*  [azure-iot-sdk-c\build_all\arduino_cc\base-libraries\AzureIoTProtocol_MQTT](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/AzureIoTProtocol_MQTT)
*  [azure-iot-sdk-c\build_all\arduino_cc\base-libraries\AzureIoTUtility](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/AzureIoTUtility)

### Contributing to the `readme.md` files

Like the AzureIoT Arduino libraries themselves, the library `readme.md` files are assembled from sources and cannot be modified directly. The directions and the sources for modifying the library `readme.md` files are to be found [here](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/readme-generator/readme.md).
