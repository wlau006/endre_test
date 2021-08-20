#!/bin/bash
sudo dd if=/dev/fmem of=basetext.txt bs=1M count=1
cp basetext.txt modtext.txt
cat basetext.txt > input.txt
counter=1
while [ $counter -le 10 ]
do
perl corrupt.pl
cat modtext.txt >> input.txt
((counter++))
done
echo input file created