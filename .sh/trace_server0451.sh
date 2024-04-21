#! /bin/bash

#****************** ПЕРЕМЕННЫЕ ******************#

#--- Пути к файлам ---#

RUN_SCRIPT_FILE_PATH=$4

LOG_FILE_PATH=$5


#--- Прочее ---#

PATTERN_1=$1
PATTERN_2=$2
PATTERN_3=$3

CLOSE_WAIT_THRESHOLD=1
PAUSE=3


#******************* ПРОЦЕДУРЫ ******************#

#--- Запуск ---#

RESTART_DUE=0

while true; do
    CLOSE_WAIT_DETECTED=$(sudo ss -tapn | grep -E -c "CLOSE-WAIT")
    EXEC_BIN_IS_RUNNING=$(pgrep -c "$PATTERN_1")
    RUN_SCRIPT_IS_RUNNING=$(pgrep -c "$PATTERN_2")
    LOG_FLUSH_SCRIPT_IS_RUNNING=$(pgrep -c "$PATTERN_3")
    
    if [ $CLOSE_WAIT_DETECTED -gt $CLOSE_WAIT_THRESHOLD ]; then
        echo "CLOSE-WAIT clogging detected" >> $LOG_FILE_PATH
        sudo pkill -9 "$PATTERN_1"
        sudo pkill -9 "$PATTERN_2"
        sudo pkill -9 "$PATTERN_3"
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
    
    sleep $PAUSE
done &
