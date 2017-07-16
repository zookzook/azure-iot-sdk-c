echo "hello world"
echo PATH: $PATH
echo ESP8266_RTOS_SDK: $ESP8266_RTOS_SDK
echo ESP8266_TOOLS: $ESP8266_TOOLS
echo pwd: $(pwd)
cd ..
dir
export PATH=$ESP8266_TOOLS/xtensa-lx106-elf/bin:$PATH
xtensa-lx106-elf-gcc --version


