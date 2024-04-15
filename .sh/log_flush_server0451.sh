#! /bin/bash

#****************** ПЕРЕМЕННЫЕ ******************#

#--- Названия файлов ---#

CONFIG_FILE_NAME=config_server0451
LOG_FILE_NAME=log_server0451


#--- Пути к файлам ---#

THIS_SCRIPT_FILE_ABS_PATH=$(readlink -f "$0")
THIS_SCRIPT_DIR_ABS_PATH=$(dirname "$THIS_SCRIPT_FILE_ABS_PATH")

CONFIG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/../.config/$CONFIG_FILE_NAME"
LOG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/../.log/$LOG_FILE_NAME"


#--- Прочее ---#

LOG_FILE_MAX_SIZE=$(grep -E -o "LOG_FILE_MAX_SIZE=[0-9]+" "$CONFIG_FILE_PATH" | grep -E -o "[0-9]+")


#******************* ПРОЦЕДУРЫ ******************#

#--- Запуск ---#

## Очистка логов сервера.
while true; do
    LOG_FILE_SIZE=$(stat "$LOG_FILE_PATH" | grep -E -o "Size: [0-9]+" | grep -E -o "[0-9]+")
    if [ $LOG_FILE_SIZE -ge $LOG_FILE_MAX_SIZE ]; then
        echo -e "---------------------------------------------------------------------------" > $LOG_FILE_PATH
        date +"Flushing server log file, date: %d.%m.%Y, time (UTC+0): %H:%M:%S" >> $LOG_FILE_PATH
        echo -e "---------------------------------------------------------------------------" >> $LOG_FILE_PATH
    fi
done &
