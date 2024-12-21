#! /bin/bash

#****************** ПЕРЕМЕННЫЕ ******************#

#--- Прочее ---#

PATTERN_1="exec_bin_server0451"
PATTERN_2="log_flush_server0451.sh"
PATTERN_3="trace_server0451.sh"


#******************* ПРОЦЕДУРЫ ******************#

#--- Вывод информации о процессах в терминал ---#

pgrep -fa "($PATTERN_1|$PATTERN_2|$PATTERN_3)"
