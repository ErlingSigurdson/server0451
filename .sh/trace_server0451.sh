#! /bin/bash

#****************** ПЕРЕМЕННЫЕ ******************#

#--- Названия файлов ---#

LAUNCH_SCRIPT_FILE_NAME=launch_server0451.sh
LOG_FILE_NAME=log_server0451


#--- Пути к файлам ---#

THIS_SCRIPT_FILE_ABS_PATH=$(readlink -f "$0")
THIS_SCRIPT_DIR_ABS_PATH=$(dirname "$THIS_SCRIPT_FILE_ABS_PATH")

LAUNCH_SCRIPT_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/$LAUNCH_SCRIPT_FILE_NAME"
LOG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/../.log/$LOG_FILE_NAME"


#--- Прочее ---#

PATTERN_1=exec_bin_server0451
PATTERN_2=launch_server0451.sh
PATTERN_3=log_flush_server0451.sh
PATTERN_4=run_server0451.sh

CLOSE_WAIT_THRESHOLD=3


#******************* ПРОЦЕДУРЫ ******************#

#--- Запуск ---#

RESTART_DUE=0

while true; do
    CLOSE_WAIT_DETECTED=$(sudo ss -tapn | grep -E -c "CLOSE-WAIT")
    EXEC_BIN_IS_RUNNING=$(pgrep -fc "$PATTERN_1")
    LAUNCH_SCRIPT_IS_RUNNING=$(pgrep -fc "$PATTERN_2")
    LOG_FLUSH_SCRIPT_IS_RUNNING=$(pgrep -fc "$PATTERN_3")
    RUN_SCRIPT_IS_RUNNING=$(pgrep -fc "$PATTERN_4")
    
    if [ $CLOSE_WAIT_DETECTED -gt $CLOSE_WAIT_THRESHOLD ]; then
        sudo pkill -f "$PATTERN_1"
        sudo pkill -f "$PATTERN_2"
        sudo pkill -f "$PATTERN_3"
        sudo pkill -f "$PATTERN_4"
        RESTART_DUE=1
    fi

    if [ $EXEC_BIN_IS_RUNNING -le 0 ] && [ $LAUNCH_SCRIPT_IS_RUNNING -le 0 ] &&\
       [ $LOG_FLUSH_SCRIPT_IS_RUNNING -le 0 ] && [ $RUN_SCRIPT_IS_RUNNING -le 0 ] &&\
       [ $RESTART_DUE -gt 0 ]; then
        $LAUNCH_SCRIPT_FILE_PATH &
        echo -e "-----------------------------------------------------------------------" >> $LOG_FILE_PATH
        date +"Restarting server due to CLOSE-WAIT clogging, date: %d.%m.%Y, time (UTC+0): %H:%M:%S" >> $LOG_FILE_PATH
        echo -e "-----------------------------------------------------------------------" >> $LOG_FILE_PATH
        RESTART_DUE=0
    fi
    
    sleep 2
done &
