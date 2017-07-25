# Delete these first two exports  for production
if [ $(hostname) = "Purl2" ]; then
    export ESP8266_TOOLS="/c/Users/RoyS/Documents/AzureIoT/workspace/esp-open-sdk" 
    export ESP8266_RTOS_SDK="/c/Users/RoyS/Documents/AzureIoT/workspace/ESP8266_RTOS_SDK" 
elif [ $(hostname) = "JoeMorello" ]; then
    export ESP8266_TOOLS="/home/roys/workspace/esp-open-sdk" 
    export ESP8266_RTOS_SDK="/home/roys/workspace/ESP8266_RTOS_SDK" 
else
	echo "!!! FAILED !!! non-production exports in sdk_fix.sh"
	exit 1
fi
export PATH=$ESP8266_TOOLS/xtensa-lx106-elf/bin:$PATH

echo "Create 'libcirom_stripped' by stripping the duplicate "time()" definition from libcirom.a"
xtensa-lx106-elf-objcopy --strip-symbol=time $ESP8266_RTOS_SDK/lib/libcirom.a $ESP8266_RTOS_SDK/lib/libcirom_stripped.a
