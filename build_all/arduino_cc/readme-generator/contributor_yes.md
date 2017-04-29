
## Contributors

You are in the right place to contribute Arduino-specific changes to the Azure C SDK.

### Please consider all Arduino libraries for your contribution

There are several Arduino-specific library sections, and if you make a change in an Arduino-specific file such as `travis.yml` or a `.ino` file, it would likely help others if you add your contribution to the corresponding file in all of the locations:

*  [azure-iot-sdk-c\build_all\arduino_cc\base-libraries\AzureIoTHub](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/AzureIoTHub)
*  [azure-iot-sdk-c\build_all\arduino_cc\base-libraries\AzureIoTProtocol_HTTP](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/AzureIoTProtocol_HTTP)
*  [azure-iot-sdk-c\build_all\arduino_cc\base-libraries\AzureIoTProtocol_MQTT](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/AzureIoTProtocol_MQTT)
*  [azure-iot-sdk-c\build_all\arduino_cc\base-libraries\AzureIoTUtility](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/AzureIoTUtility)

### Contributing to the `readme.md` files

The `readme.md` files for the various Arduino libraries are almost identical, so to avoid duplication and versioning problems, the SDK publication process assembles the readme files from sources [here](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/readme-generator).

#### Step one: Change the readme source files:

*  [AzureIoTHub title and intro](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/readme-generator/title_hub_fragment.md)
*  [AzureIoTProtocol_HTTP title and intro](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/readme-generator/title_http_fragment.md)
*  [AzureIoTProtocol_MQTT title and intro](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/readme-generator/title_mqtt_fragment.md)
*  [AzureIoTUtility title and intro](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/readme-generator/title_utility_fragment.md)
*  [Supported devices, prerequisites, and samples instructions](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/readme-generator/title_utility_fragment.md)

#### Step two: Run the readme build script

Run the `build_readmes.ps1` with the current directory at its own location. This will generate and place individual `readme.md` files where they belong in each Arduino library source tree. It will also generate similar `readme.md` files within a `built_readmes` directory; these can be ignored, and may be safely deleted.

#### Step three: Pull

Check in your readme source changes and the `readme.md` files put into the library directories by `build_readmes.ps1`, then do your pull request as usual.
