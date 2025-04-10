#! /bin/bash

#****************** ПЕРЕМЕННЫЕ ******************#

#--- Прочее ---#

PATTERN_1="exec_bin_server0451"
PATTERN_2="log_flush_server0451.sh"
PATTERN_3="trace_server0451.sh"


#******************* ПРОЦЕДУРЫ ******************#

#--- Проверка прав root ---#

if [ "$EUID" -ne 0 ]; then
    echo "Please run this script again as root."
    exit
fi


#--- Прекращение процессов ---#

pkill -f -9 "($PATTERN_1|$PATTERN_2|$PATTERN_3)"
