#! /bin/bash

#****************** ПЕРЕМЕННЫЕ ******************#

#--- Прочее ---#

## Ограничение в 15 символов обусловлено ограничениями утилиты pkill.
PATTERN_1="exec_bin_server"
PATTERN_2="run_server0451"
PATTERN_3="log_flush_serve"
PATTERN_4="trace_server045"


#******************* ПРОЦЕДУРЫ ******************#

#--- Прекращение процессов ---#

sudo pkill -ec -9 $PATTERN_1
sudo pkill -ec -9 $PATTERN_2
sudo pkill -ec -9 $PATTERN_3
sudo pkill -ec -9 $PATTERN_4
