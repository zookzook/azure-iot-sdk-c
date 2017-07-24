if [ "$#" -ne 2 ]; then
    echo "usage is copy_iot_sample_files.sh <src_dir> <dest_dir>"
    exit 1 
fi

COMPONENT_OBJS=( \
$1/iothub_client/samples/iothub_client_sample_mqtt/iothub_client_sample_mqtt.c	\
$1/iothub_client/samples/iothub_client_sample_mqtt/iothub_client_sample_mqtt.h	\
)

echo "Azure IoT sample files destination:" $2
for each_file in ${COMPONENT_OBJS[*]}; do
    echo copying $each_file
    cp $each_file $2
done
















