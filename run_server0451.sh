#! /bin/bash

#****************** ПЕРЕМЕННЫЕ ******************#

#--- Названия файлов ---#

LAUNCH_SCRIPT_FILE_NAME=launch_server0451.sh
TRACE_SCRIPT_FILE_NAME=trace_server0451.sh
CONFIG_FILE_NAME=config_server0451


#--- Пути к файлам ---#

THIS_SCRIPT_FILE_ABS_PATH=$(readlink -f "$0")
THIS_SCRIPT_DIR_ABS_PATH=$(dirname "$THIS_SCRIPT_FILE_ABS_PATH")

LAUNCH_SCRIPT_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.sh/$LAUNCH_SCRIPT_FILE_NAME"
TRACE_SCRIPT_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.sh/$TRACE_SCRIPT_FILE_NAME"
CONFIG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.config/$CONFIG_FILE_NAME"


#--- Прочее ---#

TRACE=$(grep -E -o "TRACE=[a-zA-Z]+" "$CONFIG_FILE_PATH" | grep -E -o "=.+" | grep -E -o "[a-zA-Z]+")

VALID_TRACE_1=on
VALID_TRACE_2=off


#************ ОСНОВНАЯ ЧАСТЬ СКРИПТА ************#

#--- Проверки ---#

## Проверка заданного в настроечном файле статуса опции отслеживания состояния и автоматического перезапуска сервера.
if [ "$TRACE" != "$VALID_TRACE_1" ] && [ "$TRACE" != "$VALID_TRACE_2" ] ; then
    echo -e "Valid trace option status for server0451 is not specified in the config file."
    echo -e "Please edit $CONFIG_FILE_PATH and specify a status (valid options are \"on\" and \"off\")."
    exit
fi


#--- Запуск ---#

$LAUNCH_SCRIPT_FILE_PATH

if [ "$TRACE" = "on" ]; then
    nohup $TRACE_SCRIPT_FILE_PATH > /dev/null 2>&1 &
fi


