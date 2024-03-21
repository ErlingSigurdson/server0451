#! /bin/bash

THIS_SCRIPT_FILE_ABS_PATH=$(readlink -f "$0")
THIS_SCRIPT_DIR_ABS_PATH=$(dirname "$THIS_SCRIPT_FILE_ABS_PATH")

LOOP_SCRIPT_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.sh/loop_script_server0451.sh"
ONESHOT_SCRIPT_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.sh/oneshot_script_server0451.sh"

LOG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.log/log_server0451"

read -p "Run server0451 in (l)oop or (o)neshot mode?" MODE

if [ "$MODE" = "l" ] || [ "$MODE" = "L" ]; then
    nohup $LOOP_SCRIPT_FILE_PATH >> $LOG_FILE_PATH 2>&1 &
elif [ "$MODE" = "o" ] || [ "$MODE" = "O" ]; then
    nohup $ONESHOT_SCRIPT_FILE_PATH >> $LOG_FILE_PATH 2>&1 &
else
    echo "Inser [l/L] for loop mode or [o/O] for oneshot mode".
fi

MAX_LOG_SIZE=5000000

while true; do
    LOG_SIZE=$(stat $LOG_FILE_PATH | grep -E -o "Size: [0-9]*" | grep -E -o "[0-9]*")
    if [ "$LOG_SIZE" -ge "$MAX_LOG_SIZE" ]; then
        echo "Starting new log file." > $LOG_FILE_PATH
    fi
done &
