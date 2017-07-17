if [ "$#" -ne 2 ]; then
    echo "usage is copy_iot_files SRC_DIR DEST_DIR"
    exit 1 
fi

COMPONENT_OBJS=( \
$1/c-utility/src/xlogging.c	\
$1/c-utility/src/buffer.c	\
$1/c-utility/src/consolelogger.c	\
$1/c-utility/src/constbuffer.c	\
$1/c-utility/src/constmap.c	\
$1/c-utility/src/crt_abstractions.c	\
$1/c-utility/src/doublylinkedlist.c	\
$1/c-utility/src/gballoc.c	\
$1/c-utility/src/gb_stdio.c	\
$1/c-utility/src/gb_time.c	\
$1/c-utility/src/hmac.c	\
$1/c-utility/src/hmacsha256.c	\
$1/c-utility/src/httpapiex.c	\
$1/c-utility/src/httpapiexsas.c	\
$1/c-utility/src/httpheaders.c	\
$1/c-utility/src/map.c	\
$1/c-utility/src/optionhandler.c	\
$1/c-utility/src/sastoken.c	\
$1/c-utility/src/sha1.c	\
$1/c-utility/src/sha224.c	\
$1/c-utility/src/sha384-512.c	\
$1/c-utility/src/strings.c	\
$1/c-utility/src/string_tokenizer.c	\
$1/c-utility/src/urlencode.c	\
$1/c-utility/src/usha.c	\
$1/c-utility/src/vector.c	\
$1/c-utility/src/xio.c	\
$1/c-utility/src/base64.c \
\
\
$1/iothub_client/src/iothub_client.c	\
$1/iothub_client/src/iothub_client_ll.c	\
$1/iothub_client/src/iothub_client_ll_uploadtoblob.c	\
$1/iothub_client/src/iothub_client_authorization.c	\
$1/iothub_client/src/iothub_message.c	\
$1/iothub_client/src/iothubtransport.c	\
$1/iothub_client/src/iothubtransportmqtt.c	\
$1/iothub_client/src/iothubtransport_mqtt_common.c	\
$1/iothub_client/src/version.c	\
\
\
$1/umqtt/src/mqtt_client.c	\
$1/umqtt/src/mqtt_codec.c	\
$1/umqtt/src/mqtt_message.c	\
\
\
\
$1/c-utility/adapters/agenttime.c	\
$1/c-utility/src/singlylinkedlist.c	\
$1/c-utility/adapters/platform_openssl_compact.c	\
$1/c-utility/adapters/threadapi_freertos.c	\
$1/c-utility/adapters/tickcounter_freertos.c	\
$1/c-utility/adapters/sntp_lwip.c	\
$1/c-utility/adapters/dns_async.c	\
$1/c-utility/adapters/socket_async.c	\
$1/c-utility/adapters/tlsio_openssl_compact.c	\
$1/iothub_client/samples/iothub_client_sample_mqtt/iothub_client_sample_mqtt.c	\
)

for each_file in ${COMPONENT_OBJS[*]}; do
    echo copying $each_file to $2
    cp $each_file $2
done















