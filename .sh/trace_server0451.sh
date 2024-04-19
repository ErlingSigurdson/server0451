#! /bin/bash

#****************** ПЕРЕМЕННЫЕ ******************#

#--- Названия файлов ---#

EXEC_BIN_FILE_NAME="$1"
RUN_SCRIPT_FILE_NAME="$2"
LOG_FLUSH_SCRIPT_FILE_NAME="$3"


#--- Пути к файлам ---#

RUN_SCRIPT_FILE_PATH="$4"

LOG_FILE_SCRIPT_PATH="$5"


#--- Прочее ---#

PATTERN_1="$EXEC_BIN_FILE_NAME"
PATTERN_2="$RUN_SCRIPT_FILE_NAME"
PATTERN_3="$LOG_FLUSH_SCRIPT_FILE_NAME"

CLOSE_WAIT_THRESHOLD=2
PERIOD=3


#******************* ПРОЦЕДУРЫ ******************#

#--- Запуск ---#

RESTART_DUE=0

while true; do
    CLOSE_WAIT_DETECTED=$(sudo ss -tapn | grep -E -c "CLOSE-WAIT")
    EXEC_BIN_IS_RUNNING=$(pgrep -fc "$PATTERN_1")
    RUN_SCRIPT_IS_RUNNING=$(pgrep -fc "$PATTERN_2")
    LOG_FLUSH_SCRIPT_IS_RUNNING=$(pgrep -fc "$PATTERN_3")
    
    if [ $CLOSE_WAIT_DETECTED -gt $CLOSE_WAIT_THRESHOLD ]; then
        sudo pkill -f "$PATTERN_1"
        sudo pkill -f "$PATTERN_2"
        sudo pkill -f "$PATTERN_3"
        RESTART_DUE=1
    fi

    if [ $EXEC_BIN_IS_RUNNING -le 0 ] && [ $RUN_SCRIPT_IS_RUNNING -le 0 ] &&\
       [ $LOG_FLUSH_SCRIPT_IS_RUNNING -le 0 ] && [ $RESTART_DUE -gt 0 ]; then
        $RUN_SCRIPT_FILE_PATH &
        echo -e "--------------------------------------------------------------------------------------"\
        >> $LOG_FILE_PATH
        date -u +"Restarting server due to CLOSE-WAIT clogging. Date: %d.%m.%Y, time (UTC+0): %H:%M:%S"\
        >> $LOG_FILE_PATH
        echo -e "--------------------------------------------------------------------------------------"\
        >> $LOG_FILE_PATH
        RESTART_DUE=0
        exit
    fi
    
    sleep $PERIOD
done &
