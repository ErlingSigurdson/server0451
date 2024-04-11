#! /bin/bash

#****************** ПЕРЕМЕННЫЕ ******************#

#--- Названия файлов ---#

EXEC_BIN_FILE_NAME=exec_bin_server0451
CONFIG_FILE_NAME=config_server0451
LOG_FILE_NAME=log_server0451


#--- Пути к файлам ---#

THIS_SCRIPT_FILE_ABS_PATH=$(readlink -f "$0")
THIS_SCRIPT_DIR_ABS_PATH=$(dirname "$THIS_SCRIPT_FILE_ABS_PATH")

EXEC_BIN_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/bin/$EXEC_BIN_FILE_NAME"
CONFIG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.config/$CONFIG_FILE_NAME"
LOG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.log/$LOG_FILE_NAME"


#--- Прочее ---#

PORT=$(grep -E -o "PORT=[0-9]+" "$CONFIG_FILE_PATH" | grep -E -o "[0-9]+")
PASSWORD=$(grep -E -o "PASSWORD=.{5,40}" "$CONFIG_FILE_PATH" | grep -E -o "=.+" | grep -E -o "[a-zA-Z0-9]+")
MODE=$(grep -E -o "MODE=[a-zA-Z]+" "$CONFIG_FILE_PATH" | grep -E -o "=.+" | grep -E -o "[a-zA-Z]+")

VALID_MODE_1=loop
VALID_MODE_2=oneshot

MAX_LOG_SIZE=50000000


#************ ОСНОВНАЯ ЧАСТЬ СКРИПТА ************#

#--- Проверки ---#

## Проверка на наличие исполняемого бинарного файла в соответствующей директории.
if [ ! -e "$EXEC_BIN_FILE_PATH" ]; then
    echo "Executable binary file of server0451 not found. \
    Please follow to $THIS_SCRIPT_DIR_ABS_PATH/src and run make utility."
    exit
fi

## Проверка заданного в настроечном файле порта.
if [ -z "$PORT" ]; then
    echo -e "Valid port for server0451 is not specified in the config file. \
    Please edit $CONFIG_FILE_PATH and specify a port (an integer from 0 to 65535)."
    exit
fi

## Проверка заданного в настроечном файле пароля.
if [ -z "$PASSWORD" ]; then
    echo -e "Valid password for server0451 is not specified in the config file. \
    Please edit $CONFIG_FILE_PATH and specify a password (a string of 5 to 40 ASCII alphanumerics)."
    exit
fi

## Проверка заданного в настроечном файле режима.
if [ "$MODE" != "$VALID_MODE_1" ] && [ "$MODE" != "$VALID_MODE_2" ] ; then
    echo -e "Valid operation mode for server0451 is not specified in the config file. \
    Please edit $CONFIG_FILE_PATH and specify a mode (valid options are \"loop\" and \"oneshot\")."
    exit
fi

## Создание нового файла с логами сервера, если такой файл отсутствует.
if [ ! -e "$LOG_FILE_PATH" ]; then
    echo -e "Creating a new server log file." > $LOG_FILE_PATH
fi


#--- Запуск ---#

## Запуск сервера в циклическом режиме (основной режим).
if [ "$MODE" = "loop" ]; then
    echo -e "server0451 started in a loop mode at port $PORT."
    nohup sudo $EXEC_BIN_FILE_PATH -p $PORT -P $PASSWORD -V >> $LOG_FILE_PATH 2>&1 &
fi

## Запуск сервера в режиме одиночного прогона (тестовый режим).
if [ "$MODE" = "oneshot" ]; then
    echo "server0451 started in a oneshot mode at port $PORT."
    sudo $EXEC_BIN_FILE_PATH -p $PORT -P $PASSWORD -o -V
fi

## Хранение и удаление логов сервера.
while [ "$MODE" = "loop" ]; do
    LOG_SIZE=$(stat "$LOG_FILE_PATH" | grep -E -o "Size: [0-9]+" | grep -E -o "[0-9]+")
    if [ "$LOG_SIZE" -ge "$MAX_LOG_SIZE" ]; then
        echo "Creating a new server log file." > $LOG_FILE_PATH
    fi
done &
