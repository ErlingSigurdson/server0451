#! /bin/bash

EXEC_FILEPATH="../bin/execute_server0451"
CONFIG_FILEPATH="./configs_server0451"

PORT_1=80
PORT_2=451

sudo lsof -l -P -n > tmp_1
PORT_1_BOUND=$(grep -c ":$PORT_1.*LISTEN" tmp_1)
rm tmp_1
if [ $PORT_1_BOUND -eq 0 ]; then
    sudo $EXEC_FILEPATH -p $PORT_1 -f $CONFIG_FILEPATH -v
fi
    
sudo lsof -l -P -n > tmp_2
PORT_2_BOUND=$(grep -c ":$PORT_2.*LISTEN" tmp_2)
rm tmp_2
if [ $PORT_2_BOUND -eq 0 ]; then
    sudo $EXEC_FILEPATH -p $PORT_2 -f $CONFIG_FILEPATH -v
fi
