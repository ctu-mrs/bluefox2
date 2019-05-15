#!/bin/bash

curl -O -J http://mrs.felk.cvut.cz/data/bluefox/mvBlueFOX-x86_64_ABI2-latest.tgz.version
rename_to=`cat mvBlueFOX-x86_64_ABI2-latest.tgz.version`
curl -O -J http://mrs.felk.cvut.cz/data/bluefox/mvBlueFOX-x86_64_ABI2-latest.tgz 
echo "renaming the latest to version name "$rename_to
mv mvBlueFOX-x86_64_ABI2-latest.tgz $rename_to
rm *.version