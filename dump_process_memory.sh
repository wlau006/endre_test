#!/bin/bash


sleep 2
outputfile="mbwdump.txt"
output=$(pgrep -f -n "mbw")

cat snapshot.txt > $outputfile

for k in {1..1}; do
#while kill -0 $output 2> /dev/null; do

    grep rw-p /proc/$output/maps \
    | sed -n 's/^\([0-9a-f]*\)-\([0-9a-f]*\) .*$/\1 \2/p' \
    | while read start stop; do \
        gdb --batch-silent --pid $output -ex \
            "dump binary memory $output-$start-$stop.dump 0x$start 0x$stop"; \
    done


    for i in *.dump
    do 
       cat $i >> $outputfile
       rm $i
    done
    echo "Run $k"
    sleep 1
done
