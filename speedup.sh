#!/bin/sh

TIMEFORMAT=%R

echo 
echo "Executing single"
timeSingle=$( { time ./obj/single 1>/dev/null; } 2>&1)
echo "Time single: "$timeSingle
echo "Executing multi"
timeMulti=$( { time ./obj/multi 1>/dev/null; } 2>&1)
echo "Time multi: "$timeMulti

speedup=$(echo "scale=2; $timeSingle / $timeMulti" | bc)

echo 
echo "Speedup: "$speedup
echo 

