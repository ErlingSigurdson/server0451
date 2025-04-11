#! /bin/bash

#****************** ПЕРЕМЕННЫЕ ******************#

#--- Названия файлов ---#

EXEC_BIN_FILE_NAME="exec_bin_server0451"
CONFIG_FILE_NAME="config_server0451"
LOG_FILE_NAME="log_server0451"

RUN_SCRIPT_FILE_NAME="run_server0451.sh"
LOG_FLUSH_SCRIPT_FILE_NAME="log_flush_server0451.sh"
TRACE_SCRIPT_FILE_NAME="trace_server0451.sh"

KILL_SCRIPT_FILE_NAME="pkill_server0451.sh"


#--- Пути к файлам ---#

THIS_SCRIPT_FILE_PATH=$(readlink -f "$0")
THIS_SCRIPT_DIR_PATH=$(dirname "$THIS_SCRIPT_FILE_PATH")

EXEC_BIN_FILE_PATH="$THIS_SCRIPT_DIR_PATH/.bin/$EXEC_BIN_FILE_NAME"
CONFIG_FILE_PATH="$THIS_SCRIPT_DIR_PATH/.config/$CONFIG_FILE_NAME"
LOG_FILE_PATH="$THIS_SCRIPT_DIR_PATH/.log/$LOG_FILE_NAME"

RUN_SCRIPT_FILE_PATH="$THIS_SCRIPT_FILE_PATH"
LOG_FLUSH_SCRIPT_FILE_PATH="$THIS_SCRIPT_DIR_PATH/.sh/$LOG_FLUSH_SCRIPT_FILE_NAME"
TRACE_SCRIPT_FILE_PATH="$THIS_SCRIPT_DIR_PATH/.sh/$TRACE_SCRIPT_FILE_NAME"

KILL_SCRIPT_FILE_PATH="$THIS_SCRIPT_DIR_PATH/$KILL_SCRIPT_FILE_NAME"


#--- Прочее ---#

PORT=$(grep -Eo "PORT=[0-9]+" "$CONFIG_FILE_PATH" | grep -Eo "[0-9]+")
PASSWORD=$(grep -Eo "PASSWORD=.{5,40}$" "$CONFIG_FILE_PATH" | grep -Eo "=.+" | grep -Eo "[a-zA-Z0-9]+")
MODE=$(grep -Eo "MODE=[a-zA-Z]+" "$CONFIG_FILE_PATH" | grep -Eo "=.+" | grep -Eo "[a-zA-Z]+")
LOG_FILE_MAX_SIZE=$(grep -Eo "LOG_FILE_MAX_SIZE=[0-9]+" "$CONFIG_FILE_PATH" | grep -Eo "[0-9]+")
TRACE=$(grep -Eo "TRACE=[a-zA-Z]+" "$CONFIG_FILE_PATH" | grep -Eo "=.+" | grep -Eo "[a-zA-Z]+")

VALID_MODE_1=loop
VALID_MODE_2=oneshot

VALID_TRACE_STATUS_1=on
VALID_TRACE_STATUS_2=off


#******************* ПРОЦЕДУРЫ ******************#

#--- Проверки ---#

## Проверка прав root.
if [ "$EUID" -ne 0 ]; then
    echo "Please run this script again as root."
    exit
fi

## Проверка на наличие исполняемого бинарного файла в соответствующей директории.
if [ ! -e "$EXEC_BIN_FILE_PATH" ]; then
    echo -e "Error: executable binary file of server0451 not found."
    echo -e "Please follow to $THIS_SCRIPT_DIR_ABS_PATH/src and run make utility."
    exit
fi

## Проверка заданного в настроечном файле порта.
if [ -z "$PORT" ]; then
    echo -e "Error: valid port for server0451 is not specified in the config file."
    echo -e "Please edit $CONFIG_FILE_PATH and specify a port (an integer from 0 to 65535)."
    exit
fi

## Проверка заданного в настроечном файле пароля.
if [ -z "$PASSWORD" ]; then
    echo -e "Error: valid password for server0451 is not specified in the config file."
    echo -e "Please edit $CONFIG_FILE_PATH and specify a password (a string of 5 to 40 ASCII alphanumerics)."
    exit
fi

## Проверка заданного в настроечном файле режима запуска сервера.
if [ "$MODE" != "$VALID_MODE_1" ] && [ "$MODE" != "$VALID_MODE_2" ]; then
    echo -e "Error: valid operation mode for server0451 is not specified in the config file."
    echo -e "Please edit $CONFIG_FILE_PATH and specify a mode (valid options are \"loop\" and \"oneshot\")."
    exit
fi

## Проверка заданного в настроечном файле максимального размера файла с логами сервера.
if [ -z "$LOG_FILE_MAX_SIZE" ]; then
    echo -e "Erorr: maximum log file size for server0451 is not specified in the config file."
    echo -e "Please edit $CONFIG_FILE_PATH and specify a maximum file size (in bytes)."
    exit
fi

## Проверка заданного в настроечном файле статуса автоматического отслеживания CLOSE-WAIT'ов.
if [ "$TRACE" != "$VALID_TRACE_STATUS_1" ] && [ "$TRACE" != "$VALID_TRACE_STATUS_2" ]; then
    echo -e "Error: valid status for CLOSE-WAIT tracing script is not specified in the config file."
    echo -e "Please edit $CONFIG_FILE_PATH and specify a status (valid options are \"on\" and \"off\")."
    exit
fi

## Создание нового файла с логами сервера, если такой файл отсутствует.
if [ ! -e "$LOG_FILE_PATH" ]; then
        echo -e "------------------------------------------------------------------------" > $LOG_FILE_PATH
        date -u +"Creating a new server log file. Date: %d.%m.%Y, time (UTC+0): %H:%M:%S" >> $LOG_FILE_PATH
        echo -e "------------------------------------------------------------------------" >> $LOG_FILE_PATH
fi


#--- Остановка ранее запущенного сервера ---#

$KILL_SCRIPT_FILE_PATH > /dev/null 2>&1


#--- Запуск сервера ---#

## Запуск сервера в циклическом режиме (основной режим).
if [ "$MODE" = "loop" ]; then
    echo -e "server0451 started in a loop mode at port $PORT."
    nohup $EXEC_BIN_FILE_PATH -p $PORT -P $PASSWORD -V >> $LOG_FILE_PATH 2>&1 &
fi

## Запуск сервера в режиме одиночного прогона (тестовый режим).
if [ "$MODE" = "oneshot" ]; then
    echo "server0451 started in a oneshot mode at port $PORT."
    $EXEC_BIN_FILE_PATH -p $PORT -P $PASSWORD -o -V
fi

## Запуск скрипта очистки логов сервера.
if [ "$MODE" = "loop" ]; then
    nohup $LOG_FLUSH_SCRIPT_FILE_PATH $LOG_FILE_MAX_SIZE $LOG_FILE_PATH > /dev/null 2>&1 &
fi

## Запуск скрипта автоматического отслеживания CLOSE-WAIT'ов.
if [ "$TRACE" = "on" ]; then
    nohup $TRACE_SCRIPT_FILE_PATH\
          "exec_bin_server0451" "run_server0451.sh" "log_flush_server0451.sh"\
          $RUN_SCRIPT_FILE_PATH $LOG_FILE_PATH $PORT\
          > /dev/null 2>&1 &
fi
