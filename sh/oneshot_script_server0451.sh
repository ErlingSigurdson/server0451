#! /bin/bash

EXEC_FILEPATH="../bin/execute_server0451"
CONFIG_FILEPATH="../bin/configs_server0451"

sudo $EXEC_FILEPATH -p 80 -f $CONFIG_FILEPATH -v
sudo $EXEC_FILEPATH -p 451 -f $CONFIG_FILEPATH -v
