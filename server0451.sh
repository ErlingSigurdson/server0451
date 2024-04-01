#! /bin/bash

# Пути к файлам и прочие переменные.

THIS_SCRIPT_FILE_ABS_PATH=$(readlink -f "$0")
THIS_SCRIPT_DIR_ABS_PATH=$(dirname "$THIS_SCRIPT_FILE_ABS_PATH")

EXEC_BIN_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/bin/execute_server0451"

PORT_CONFIG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.config/port_config_server0451"
PORT_1=$(grep -E -o "PORT_1=[0-9]+" "$PORT_CONFIG_FILE_PATH" | grep -E -o "=[0-9]+" | grep -E -o "[0-9]+")
PORT_2=$(grep -E -o "PORT_2=[0-9]+" "$PORT_CONFIG_FILE_PATH" | grep -E -o "=[0-9]+" | grep -E -o "[0-9]+")

PASSWORD_CONFIG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.config/password_config_server0451"
PASSWORD=$(grep -E -o ".{5,40}" "$PASSWORD_CONFIG_FILE_PATH")

LOOP_SCRIPT_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.sh/loop_script_server0451.sh"
ONESHOT_SCRIPT_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.sh/oneshot_script_server0451.sh"

LOG_FILE_PATH="$THIS_SCRIPT_DIR_ABS_PATH/.log/log_server0451"
MAX_LOG_SIZE=50000000


# Основная часть скрипта.

### Проверка на наличие исполняемого бинарного файла в соответствующей директории.
if [ ! -e "$EXEC_BIN_FILE_PATH" ]; then
    echo "Binary executable file is unavailable. Go to /server0451/src directory and run make utility."
    exit;
fi

### Выбор режима работы сервера.
read -p "Do you want to run server0451 in a (l)oop or a (o)neshot mode?" MODE

### Если файл с паролем отсутствует или пароль в нём не задан, пароль нужно будет задать из командной строки.
if [ -z "${PASSWORD}" ]; then
    read -p "Create a password. All messages sent to the server will have to start with it." PASSWORD
fi

### Запуск сервера в циклическом режиме (основной режим).
if [ "$MODE" = "l" ] || [ "$MODE" = "L" ]; then
    echo "server0451 started in a loop mode at ports $PORT_1 and $PORT_2."
    nohup $LOOP_SCRIPT_FILE_PATH $EXEC_BIN_FILE_PATH $PORT_1 $PORT_2 $PASSWORD $LOG_FILE_PATH $MAX_LOG_SIZE >> $LOG_FILE_PATH 2>&1 &

### Запуск сервера в режиме одиночного прогона (тестовый режим).
elif [ "$MODE" = "o" ] || [ "$MODE" = "O" ]; then
    echo "server0451 started in a oneshot mode at ports $PORT_1 and $PORT_2."
    $ONESHOT_SCRIPT_FILE_PATH $EXEC_BIN_FILE_PATH $PORT_1 $PORT_2 $PASSWORD
else
    echo "Try again and insert [l/L] for loop mode or [o/O] for oneshot mode".
fi
