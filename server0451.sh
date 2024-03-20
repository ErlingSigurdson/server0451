#! /bin/bash

LOOP_SCRIPT_PATH="./sh/loop_script_server0451.sh"
ONESHOT_SCRIPT_PATH="./sh/oneshot_script_server0451.sh"

LOG_FILE_PATH="./log/log_server0451"

nohup $LOOP_SCRIPT_PATH >> $LOG_FILE_PATH 2>&1 &
