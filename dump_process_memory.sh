#!/bin/bash

output=$(pgrep -f -n "sysbench")

cat snapshot.txt > memorydump.txt

while kill -0 $output 2> /dev/null; do

    grep rw-p /proc/$output/maps \
    | sed -n 's/^\([0-9a-f]*\)-\([0-9a-f]*\) .*$/\1 \2/p' \
    | while read start stop; do \
        gdb --batch --pid $output -ex \
            "dump binary memory $output-$start-$stop.dump 0x$start 0x$stop"; \
    done


    for i in *.dump
    do 
       cat $i >> memorydump.txt
    done


    for i in *.dump
    do 
        rm $i
    done

    sleep 2
done