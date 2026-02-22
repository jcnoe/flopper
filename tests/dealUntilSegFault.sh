#!/bin/bash

LOG="flopper_run.log"
COUNT=0

echo "Starting flopper.exe stress run..." > "$LOG"

while true; do
    COUNT=$((COUNT+1))
    echo "Run #$COUNT" >> "$LOG"

    .././flopper.exe >> "$LOG" 2>&1
    STATUS=$?

    # 139 = 128 + 11 (SIGSEGV)
    if [ $STATUS -eq 139 ]; then
        echo "Segmentation fault detected on run #$COUNT" >> "$LOG"
        echo "Stopped after $COUNT runs due to segfault."
        break
    fi
done
