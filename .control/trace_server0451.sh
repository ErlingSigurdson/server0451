#! /bin/bash

#****************** ПЕРЕМЕННЫЕ ******************#

#--- Названия файлов ---#

MAIN_SCRIPT_FILE_NAME=run_server0451.sh
TRACE_FILE_NAME=trace_log_server0451


#--- Пути к файлам ---#

THIS_SCRIPT_FILE_ABS_PATH=$(readlink -f "$0")
THIS_SCRIPT_DIR_ABS_PATH=$(dirname "$THIS_SCRIPT_FILE_ABS_PATH")

MAIN_SCRIPT_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/../$MAIN_SCRIPT_FILE_NAME"
TRACE_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/$TRACE_FILE_NAME"


#--- Прочее ---#

PATTERN_1=exec_bin_server0451
PATTERN_2=run_server0451

CLOSE_WAIT_THRESHOLD=10


#************ ОСНОВНАЯ ЧАСТЬ СКРИПТА ************#

#--- Проверки ---#

## Создание нового файла с логами перезапуска сервера, если такой файл отсутствует.
if [ ! -e "$TRACE_FILE_PATH" ]; then
    touch $TRACE_FILE_PATH
fi


#--- Запуск ---#

RESTART_DUE=0

while true; do
    CLOSE_WAIT_DETECTED=$(sudo ss -tapn | grep -E -c "CLOSE-WAIT")
    SERVER_IS_RUNNING=$(pgrep -f -c "$PATTERN_1")
    
    if [ $CLOSE_WAIT_DETECTED -gt $CLOSE_WAIT_THRESHOLD ]; then
        sudo pkill -f "$PATTERN_1"
        sudo pkill -f "$PATTERN_2"
        RESTART_DUE=1
    fi

    if [ $SERVER_IS_RUNNING -le 0 ] && [ $RESTART_DUE -gt 0 ]; then
        $MAIN_SCRIPT_FILE_PATH &
        date +"server0451 restarted due to CLOSE-WAIT clogging, %H:%M:%S %d.%m.%Y" >> $TRACE_FILE_PATH
        RESTART_DUE=0
    fi
    
    sleep 2
done &
