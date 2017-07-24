# Delete these first two exports  for production
if [ $(hostname) = "Purl2" ]; then
    export ESP8266_TOOLS="/c/Users/RoyS/Documents/AzureIoT/workspace/esp-open-sdk" 
    export ESP8266_RTOS_SDK="/c/Users/RoyS/Documents/AzureIoT/workspace/ESP8266_RTOS_SDK" 
elif [ $(hostname) = "JoeMorello" ]; then
    export ESP8266_TOOLS="/home/roys/workspace/esp-open-sdk" 
    export ESP8266_RTOS_SDK="/home/roys/workspace/ESP8266_RTOS_SDK" 
else
	echo "!!! FAILED !!! non-production exports in build.sh"
	exit 1
fi


WORKSPACE_DIR=$( echo $ESP8266_RTOS_SDK | rev | cut -d'/' -f2- | rev)
cd $WORKSPACE_DIR
export AZURE_SDK=$WORKSPACE_DIR/ESP8266
BUILD_DIR=$WORKSPACE_DIR/build
PROJECT_DIR=$BUILD_DIR/project
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
BINARY_DIR=$BUILD_DIR/bin
echo "Constructing in" $BUILD_DIR
echo "Building in" $BINARY_DIR
mkdir $BINARY_DIR

if $AZURE_SDK/build_all/esp8266/construct_build_tree.sh $AZURE_SDK $PROJECT_DIR
    then echo "build tree constructed"
else 
    echo "!!!FAILED!!! build tree construction failed"
    exit 1
fi

# exports needed for the Espressif ESP8266 make process
export SDK_PATH=$ESP8266_RTOS_SDK
export BIN_PATH=$BINARY_DIR
export PATH=$ESP8266_TOOLS/xtensa-lx106-elf/bin:$PATH
cd $PROJECT_DIR
if make BOOT=none APP=0 SPI_SPEED=40 SPI_MODE=QIO SPI_SIZE_MAP=6
    then echo "Build succeeded!"
else
    echo "Build failed during make process"
    exit 1
fi
