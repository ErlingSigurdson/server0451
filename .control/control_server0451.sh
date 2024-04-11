#! /bin/bash

#****************** ПЕРЕМЕННЫЕ ******************#

#--- Названия файлов ---#

MAIN_SCRIPT_FILE_NAME=run_server0451.sh
TRACE_FILE_NAME=trace_server0451


#--- Пути к файлам ---#

THIS_SCRIPT_FILE_ABS_PATH=$(readlink -f "$0")
THIS_SCRIPT_DIR_ABS_PATH=$(dirname "$THIS_SCRIPT_FILE_ABS_PATH")

MAIN_SCRIPT_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/../$MAIN_SCRIPT_FILE_NAME"
TRACE_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/$TRACE_FILE_NAME"


#--- Прочее ---#

PKILL_PATTERN_1=exec_bin_server0451
PKILL_PATTERN_2=run_server0451

CLOSE_WAIT_THRESHOLD=10


#************ ОСНОВНАЯ ЧАСТЬ СКРИПТА ************#

#--- Проверки ---#

## Создание нового файла с логами перезапуска сервера, если такой файл отсутствует.
if [ ! -e "$TRACE_FILE_PATH" ]; then
    touch $TRACE_FILE_PATH
fi


#--- Запуск ---#

while true; do
    CLOSE_WAIT_DETECTED=$(sudo ss -tapn | grep -E -c "CLOSE-WAIT")

    if [ $CLOSE_WAIT_DETECTED -gt $CLOSE_WAIT_THRESHOLD ]; then
        sudo pkill "$PKILL_PATTERN_1"
        sudo pkill "$PKILL_PATTERN_2"
        sleep 2

        $MAIN_SCRIPT_FILE_PATH
        date +"Server restarted due to CLOSE-WAIT clogging, %H:%M:%S %d.%m.%Y" >> $TRACE_FILE_PATH
    fi
    
    sleep 2
done &
