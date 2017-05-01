
# Contributing to the `readme.md` files for the AzureIoT Arduino libraries.

Arduino libraries are handled very differently from standard C libraries, and the organization of the AzureIoT Arduino library `readme.md` files reflects this mismatch. The AzureIoT Arduino library `readme.md` files are very similar from one library to the next, yet the `readme.md` for any given library must be different in the sources location (here) than in the assembled target location. So to avoid duplication and versioning problems, the SDK publication process assembles the readme files from the sources located here.
### Step one: Change the readme source files:

*  [AzureIoTHub title and intro](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/readme-generator/title_hub_fragment.md)
*  [AzureIoTProtocol_HTTP title and intro](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/readme-generator/title_http_fragment.md)
*  [AzureIoTProtocol_MQTT title and intro](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/readme-generator/title_mqtt_fragment.md)
*  [AzureIoTUtility title and intro](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/readme-generator/title_utility_fragment.md)
*  [Supported devices, prerequisites, and samples instructions](https://github.com/Azure/azure-iot-sdk-c/tree/master/build_all/arduino_cc/base-libraries/readme-generator/title_utility_fragment.md)

### Step two: Run the readme build script

Run the `build_readmes.ps1` with the current directory at its own location. This will generate and place individual `readme.md` files where they belong in each Arduino library source tree. It will also generate a similar set of `readme.md` files within a `built_readmes` directory. The `built_readmes` directory can be ignored, and may be safely deleted as it only used in the SDK publication process.

### Step three: Pull

Check in your readme source changes and the `readme.md` files put into the library directories by `build_readmes.ps1`, then do your pull request as usual.
