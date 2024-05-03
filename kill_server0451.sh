#! /bin/bash

#****************** ПЕРЕМЕННЫЕ ******************#

#--- Прочее ---#

## Ограничение в 15 символов обусловлено ограничениями утилиты pkill.
PATTERN_1="exec_bin_server"
PATTERN_2="log_flush_serve"
PATTERN_3="trace_server045"


#******************* ПРОЦЕДУРЫ ******************#

#--- Прекращение процессов ---#

sudo pkill -ec -9 $PATTERN_1
sudo pkill -ec -9 $PATTERN_2
sudo pkill -ec -9 $PATTERN_3
