#!/bin/bash

curl  -O -J http://mrs.felk.cvut.cz/data/bluefox/mvBlueFOX-ARMhf_gnueabi-latest.tgz.version
rename_to=`cat mvBlueFOX-ARMhf_gnueabi-latest.tgz.version`
curl -O -J http://mrs.felk.cvut.cz/data/bluefox/mvBlueFOX-ARMhf_gnueabi-latest.tgz 
echo "renaming the latest to version name "$rename_to
mv mvBlueFOX-ARMhf_gnueabi-latest.tgz $rename_to
rm *.version