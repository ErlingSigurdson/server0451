#! /bin/bash

EXEC_FILEPATH="../bin/execute_server0451"
CONFIG_FILEPATH="./configs_server0451"

while true
do
    sudo $EXEC_FILEPATH -p 80 -f $CONFIG_FILEPATH
done &

while true
do
    sudo $EXEC_FILEPATH -p 451 -f $CONFIG_FILEPATH
done &
