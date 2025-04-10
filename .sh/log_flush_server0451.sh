#! /bin/bash

#****************** ПЕРЕМЕННЫЕ ******************#

#--- Пути к файлам ---#

LOG_FILE_PATH="$2"


#--- Прочее ---#

LOG_FILE_MAX_SIZE=$1


#******************* ПРОЦЕДУРЫ ******************#

#--- Очистка логов сервера ---#
while true; do
    LOG_FILE_SIZE=$(stat "$LOG_FILE_PATH" | grep -Eo "Size: [0-9]+" | grep -Eo "[0-9]+")
    if [ $LOG_FILE_SIZE -ge $LOG_FILE_MAX_SIZE ]; then
        echo -e "-------------------------------------------------------------------" > $LOG_FILE_PATH
        echo -e "              Size limit for server log file reached." >> $LOG_FILE_PATH
        date -u +"Flushing server log file. Date: %d.%m.%Y, time (UTC+0): %H:%M:%S" >> $LOG_FILE_PATH
        echo -e "-------------------------------------------------------------------" >> $LOG_FILE_PATH
    fi
done &
