#! /bin/bash

while true
do
    sudo $1 -p $2 -P $4 -v
done &

while true
do
    sudo $1 -p $3 -P $4 -v
done &

while true; do
    LOG_SIZE=$(stat "$5" | grep -E -o "Size: [0-9]+" | grep -E -o "[0-9]+")
    if [ "$LOG_SIZE" -ge "$6" ]; then
        echo "Creating a new log file." > $5
    fi
done &
