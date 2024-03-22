#! /bin/bash

THIS_SCRIPT_FILE_ABS_PATH=$(readlink -f "$0")
THIS_SCRIPT_DIR_ABS_PATH=$(dirname "$THIS_SCRIPT_FILE_ABS_PATH")

EXEC_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/bin/execute_server0451"

PORT_CONFIG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.config/port_config_server0451"
PORT_1=$(grep -E -o "PORT_1=[0-9]+" "$PORT_CONFIG_FILE_PATH" | grep -E -o "=[0-9]+" | grep -E -o "[0-9]+")
PORT_2=$(grep -E -o "PORT_2=[0-9]+" "$PORT_CONFIG_FILE_PATH" | grep -E -o "=[0-9]+" | grep -E -o "[0-9]+")

LOOP_SCRIPT_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.sh/loop_script_server0451.sh"
ONESHOT_SCRIPT_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.sh/oneshot_script_server0451.sh"

LOG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.log/log_server0451"

if [ ! -e "$EXEC_FILE_PATH" ]; then
    echo "Program executable file is unavailable. Move to /server0451/src directory and run make utility."
    exit;
fi

read -p "Do you want to run server0451 in a (l)oop or a (o)neshot mode?" MODE

if [ "$MODE" = "l" ] || [ "$MODE" = "L" ]; then
    nohup $LOOP_SCRIPT_FILE_PATH >> $LOG_FILE_PATH 2>&1 &
    echo "server0451 started in a loop mode at ports $PORT_1 and $PORT_2."
elif [ "$MODE" = "o" ] || [ "$MODE" = "O" ]; then
    nohup $ONESHOT_SCRIPT_FILE_PATH >> $LOG_FILE_PATH 2>&1 &
    echo "server0451 started in a oneshot mode at ports $PORT_1 and $PORT_2."
else
    echo "Try again and insert [l/L] for loop mode or [o/O] for oneshot mode".
fi
