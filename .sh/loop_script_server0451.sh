#! /bin/bash

THIS_SCRIPT_FILE_ABS_PATH=$(readlink -f "$0")
THIS_SCRIPT_DIR_ABS_PATH=$(dirname "$THIS_SCRIPT_FILE_ABS_PATH")

EXEC_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/../bin/execute_server0451"

PORT_CONFIG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/../.config/port_config_server0451"
PORT_1=$(grep -E -o "PORT_1=[0-9]+" "$PORT_CONFIG_FILE_PATH" | grep -E -o "=[0-9]+" | grep -E -o "[0-9]+")
PORT_2=$(grep -E -o "PORT_2=[0-9]+" "$PORT_CONFIG_FILE_PATH" | grep -E -o "=[0-9]+" | grep -E -o "[0-9]+")

while true
do
    sudo $EXEC_FILE_PATH -p $PORT_1 -k $1 -v
done &

while true
do
    sudo $EXEC_FILE_PATH -p $PORT_2 -k $1 -v
done &

LOG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/../.log/log_server0451"

MAX_LOG_SIZE=50000000

while true; do
    LOG_SIZE=$(stat $LOG_FILE_PATH | grep -E -o "Size: [0-9]+" | grep -E -o "[0-9]+")
    if [ "$LOG_SIZE" -ge "$MAX_LOG_SIZE" ]; then
        echo "Creating a new log file." > $LOG_FILE_PATH
    fi
done &
