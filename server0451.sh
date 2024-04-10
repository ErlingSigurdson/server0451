#! /bin/bash

#--- Переменные ---#

## Пути к файлам.
THIS_SCRIPT_FILE_ABS_PATH=$(readlink -f "$0")
THIS_SCRIPT_DIR_ABS_PATH=$(dirname "$THIS_SCRIPT_FILE_ABS_PATH")

EXEC_BIN_FILE_NAME="execute_server0451"
CONFIG_FILE_NAME="config_server0451"

EXEC_BIN_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/bin/execute_server0451"
CONFIG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.config/config_server0451"

### Файл с логами сервера.
SERVER_LOG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.log/server_log_server0451"

### Файл с логами скрипта, перезапускающего сервер в случае необходимости.
CONTROL_LOG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.log/server_log_server0451"


## Прочие переменные.
PASSWORD=$(grep -E -o "PASSWORD=.{5,40}" "$CONFIG_FILE_PATH" | grep -E -o "=.+" | grep -E -o "[a-zA-Z0-9]+")
PORT=$(grep -E -o "PORT=[0-9]+" "$CONFIG_FILE_PATH" | grep -E -o "[0-9]+")
MODE=$(grep -E -o "MODE=[a-zA-Z]+" "$CONFIG_FILE_PATH" | grep -E -o "=.+" | grep -E -o "[a-zA-Z]+")

MAX_LOG_SIZE=50000000


#--- ОСНОВНАЯ ЧАСТЬ ---#

## Проверки.

### Проверка на наличие исполняемого бинарного файла в соответствующей директории.
if [ ! -e "$EXEC_BIN_FILE_PATH" ]; then
    echo "Binary executable file of server0451 not found. Go to '/server0451/src' directory and run 'make' utility."
    exit
fi

### Проверка на заданный в настроечном файле пароль.
if [ -z "${PASSWORD}" ]; then
    echo "Password for server0451 is not specified in the config file. Please open '/server0451/.config/config_server0451' and specify a password."
    exit
fi

### Проверка на заданный в настроечном файле порт.
if [ -z "${PASSWORD}" ]; then
    echo "Port for server0451 is not specified in the config file. Please open '/server0451/.config/config_server0451' and specify a port."
    exit
fi

### Проверка на заданный в настроечном файле режим.
if [ -z "${MODE}" ]; then
    echo "server0451 operation mode is not specified in the config file. 
    Please open '/server0451/.config/config_server0451' and specify a mode (valid oprtions are 'loop' and 'oneshot')."

    exit
fi

### Создание нового файла с логами сервера.
echo "Creating a new server log file" > "$CONFIG_FILE_PATH"

### Создание нового файла с логами контроля.
echo "Creating a new server log file" > "$CONFIG_FILE_PATH"


## Исполнение.

### Запуск сервера в циклическом режиме (основной режим).
if [ "$MODE" = "l" ] || [ "$MODE" = "L" ]; then
    echo "server0451 started in a loop mode at port $PORT."
    nohup sudo $EXEC_BIN_FILE_PATH -p $PORT -P $PASSWORD -V >> $LOG_FILE_PATH 2>&1 &

### Запуск сервера в режиме одиночного прогона (тестовый режим).
#elif [ "$MODE" = "o" ] || [ "$MODE" = "O" ]; then
#    echo "server0451 started in a oneshot mode at port $PORT."
#    sudo $EXEC_BIN_FILE_PATH -p $PORT -P $PASSWORD -o -V
#else
#    echo "Try again and insert [l/L] for loop mode or [o/O] for oneshot mode".
#fi


## Контроль.

### Хранение и удаление логов сервера.
while [ "$MODE" = "l" ] || [ "$MODE" = "L" ]; do
    LOG_SIZE=$(stat "$LOG_FILE_PATH" | grep -E -o "Size: [0-9]+" | grep -E -o "[0-9]+")
    if [ "$LOG_SIZE" -ge "$MAX_LOG_SIZE" ]; then
        echo "Creating a new log file." > $LOG_FILE_PATH
    fi
done &

### Контроль перезапуска сервера.
while true; do
    TMP_PS_FILE_PATH="$THIS_SCRIPT_FILE_ABS_PATH/tmp_ps"
    ps -ef > "$TMP_PS_FILE_PATH"
    SERVER0451_IS_RUNNING=$(grep -E -c "execute_server0451" "$TMP_PS_FILE_PATH")
    rm "$TMP_PS_FILE_PATH"

    if [ $SERVER0451_IS_RUNNING -le 0 ]; then
        nohup sudo $HOME/IT_playground/server0451/bin/execute_server0451 -p 451 -P admin -V
        echo -E "yes" >> trace_restart
    fi
    
    sudo ss -tapn > tmp_ss
    CLOSE_WAIT_DETECTED=$(grep -E -c "CLOSE-WAIT" tmp_ss)
    rm tmp_ss



    if [ $CLOSE_WAIT_DETECTED -gt 1 ]; then
        sudo pkill -f "execute_server0451"
        echo -E "yes" >> trace_close_wait
        echo "huy"
    fi
    
    sleep 2
done &
