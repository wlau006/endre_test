#!/bin/bash
< /dev/urandom tr -dc "[:space:][:print:]" | head -c1000000 > basetext.txt
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