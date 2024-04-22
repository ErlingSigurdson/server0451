#! /bin/bash

#****************** ПЕРЕМЕННЫЕ ******************#

#--- Прочее ---#

PATTERN_1="exec_bin_server"
PATTERN_2="run_server0451"
PATTERN_3="log_flush_serve"
PATTERN_4="trace_script_se"


#******************* ПРОЦЕДУРЫ ******************#

#--- Прекращение процессов ---#

sudo pkill -ec -9 $PATTERN_1
sudo pkill -ec -9 $PATTERN_2
sudo pkill -ec -9 $PATTERN_3
sudo pkill -ec -9 $PATTERN_4
