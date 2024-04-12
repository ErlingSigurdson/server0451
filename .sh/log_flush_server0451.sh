#! /bin/bash

#****************** ПЕРЕМЕННЫЕ ******************#

#--- Названия файлов ---#

LOG_FILE_NAME=log_server0451


#--- Пути к файлам ---#

THIS_SCRIPT_FILE_ABS_PATH=$(readlink -f "$0")
THIS_SCRIPT_DIR_ABS_PATH=$(dirname "$THIS_SCRIPT_FILE_ABS_PATH")

LOG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/../.log/$LOG_FILE_NAME"


#--- Прочее ---#

LOG_FILE_MAX_SIZE=$1


#************ ОСНОВНАЯ ЧАСТЬ СКРИПТА ************#

#--- Запуск ---#

## Очистка логов сервера.
while true; do
    LOG_FILE_SIZE=$(stat "$LOG_FILE_PATH" | grep -E -o "Size: [0-9]+" | grep -E -o "[0-9]+")
    if [ "$LOG_FILE_SIZE" -ge "$LOG_FILE_MAX_SIZE" ]; then
        date +"Creating a new server log file, date: %d.%m.%Y, time (UTC+0): %H:%M:%S" >> $LOG_FILE_PATH
    fi
done &
