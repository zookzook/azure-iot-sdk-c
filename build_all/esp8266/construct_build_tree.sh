if [ "$#" -ne 2 ]; then
    echo "!!!FAILED!!! usage is construct_build_tree.sh <azure c sdk> <build directory>"
    exit 1 
fi
echo "sdk:" $1
echo "dest:" $2

# This script must start with its current directory as the jenkins "workspace"
this_dir=${PWD##*/}
if [ $this_dir = "workspace" ]
    then echo "Creating ESP8266 build tree under 'workspace'"
else 
    echo "!!!FAILED!!! make_build_tree.sh must start in the 'workspace' directory"
    exit 1 
fi

BUILD_PROJECT=$2
BUILD_SOURCES_DIR=$BUILD_PROJECT/azure_iot
BUILD_USER_DIR=$BUILD_PROJECT/user

# We will build the project in workspace/build
if mkdir $BUILD_PROJECT && mkdir $BUILD_SOURCES_DIR && mkdir $BUILD_USER_DIR
    then echo "Created build directories"
else
    echo "Failed creating build directories"
    exit 1
fi

# The ESP8266 toolchain is incapable of working with files outside of a single source
# tree, so we need to build a tree containing all the sources we need.

# Copy the project files
if cp $1/build_all/esp8266/Makefile $BUILD_PROJECT && \
   cp -a $1/build_all/esp8266/user/. $BUILD_USER_DIR && \
   cp -a $1/build_all/esp8266/azure_iot/. $BUILD_SOURCES_DIR
   then echo "Copied project files"
else
    echo "Failed copying project files"
    exit 1
fi

# Now copy the necessary Azure IoT SDK files
if $1/build_all/esp8266/copy_iot_files.sh $1 $BUILD_SOURCES_DIR
    then echo "Azure IoT source files copied"
else 
    echo "!!!FAILED!!! Azure IoT source files copy failed"
    exit 1
fi
