#! /bin/bash

THIS_SCRIPT_FILE_ABS_PATH=$(readlink -f "$0")
THIS_SCRIPT_DIR_ABS_PATH=$(dirname "$THIS_SCRIPT_FILE_ABS_PATH")

EXEC_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/../bin/execute_server0451"

PORT_CONFIG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/../.config/ports_server0451"
PORT_1=$(grep -E -o "PORT_1=[0-9]+" "$PORT_CONFIG_FILE_PATH" | grep -E -o "=[0-9]+" | grep -E -o "[0-9]+")
PORT_2=$(grep -E -o "PORT_2=[0-9]+" "$PORT_CONFIG_FILE_PATH" | grep -E -o "=[0-9]+" | grep -E -o "[0-9]+")

CONFIG_FILE_NAME="configs_server0451"

TMP_1="$THIS_SCRIPT_DIR_ABS_PATH/tmp_1"
TMP_2="$THIS_SCRIPT_DIR_ABS_PATH/tmp_2"

sudo lsof -l -P -n > $TMP_1
PORT_1_BOUND=$(grep -c ":$PORT_1.*LISTEN" $TMP_1)
rm $TMP_1
if [ $PORT_1_BOUND -eq 0 ]; then
    sudo $EXEC_FILE_PATH -p $PORT_1 -f $CONFIG_FILE_NAME -v
fi
    
sudo lsof -l -P -n > $TMP_2
PORT_2_BOUND=$(grep -c ":$PORT_2.*LISTEN" $TMP_2)
rm $TMP_2
if [ $PORT_2_BOUND -eq 0 ]; then
    sudo $EXEC_FILE_PATH -p $PORT_2 -f $CONFIG_FILE_NAME -v
fi
