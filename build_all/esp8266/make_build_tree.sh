# This script must start with its current directory as the jenkins "workspace"
this_dir=${PWD##*/}
if [ $this_dir = "workspace" ]
    then echo "Creating ESP8266 build tree under 'workspace'"
else 
    echo "!!!FAILED!!! make_build_tree.sh must start in the 'workspace' directory"
    exit 1 
fi

# Build the project in workspace/build
rm -rf build
mkdir build
mkdir build/project
mkdir build/output
BUILD_TREE_DIR=build/project/azure_iot
mkdir $BUILD_TREE_DIR

cp -a ESP8266/build_all/esp8266/azure_iot/. $BUILD_TREE_DIR

sdk=ESP8266

ESP8266/build_all/esp8266/copy_iot_files.sh $sdk $BUILD_TREE_DIR

if [ $? = 0 ]
    then echo "Azure IoT source files copied"
else 
    echo "!!!FAILED!!! Azure IoT source files copy failed"
    exit 1
fi
