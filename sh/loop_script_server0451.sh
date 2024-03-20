#! /bin/bash

SCRIPT_FILE_ABS_PATH=$(readlink -f "$0")
SCRIPT_DIR_ABS_PATH=$(dirname "$SCRIPT_FILE_ABS_PATH")

EXEC_FILE_PATH="$SCRIPT_DIR_ABS_PATH/../bin/execute_server0451"
CONFIG_FILE_NAME="configs_server0451"

TMP_1="$SCRIPT_DIR_ABS_PATH/tmp_1"
TMP_2="$SCRIPT_DIR_ABS_PATH/tmp_2"

LOG_FILE_PATH="$SCRIPT_DIR_ABS_PATH/../log/"

PORT_1=80
PORT_2=451

while true
do
    sudo lsof -l -P -n > $TMP_1
    PORT_1_BOUND=$(grep -c ":$PORT_1.*LISTEN" $TMP_1)
    rm $TMP_1
    
    if [ $PORT_1_BOUND -eq 0 ]; then
        sudo $EXEC_FILE_PATH -p $PORT_1 -f $CONFIG_FILE_NAME -v
    fi
done &

while true
do
    sudo lsof -l -P -n > $TMP_2
    PORT_2_BOUND=$(grep -c ":$PORT_2.*LISTEN" $TMP_2)
    rm $TMP_2
    
    if [ $PORT_2_BOUND -eq 0 ]; then
        sudo $EXEC_FILE_PATH -p $PORT_2 -f $CONFIG_FILE_NAME
    fi
done &
