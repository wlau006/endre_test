#!/bin/bash
sudo dd if=/dev/fmem of=basetext.txt bs=5M count=1
cat basetext.txt > input.txt
counter=1
while [ $counter -le 10 ]
do
sleep 10s
sudo dd if=/dev/fmem of=basetext.txt bs=5M count=1
cat basetext.txt >> input.txt
((counter++))
done
echo input file created
