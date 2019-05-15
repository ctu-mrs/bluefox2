#!/bin/bash
# author: Tomas Baca

curl -O -J http://mrs.felk.cvut.cz/data/bluefox/mvBlueFOX-ARM64_gnu-latest.tgz.version
rename_to=`cat mvBlueFOX-ARM64_gnu-latest.tgz.version`
curl -O -J http://mrs.felk.cvut.cz/data/bluefox/mvBlueFOX-ARM64_gnu-latest.tgz 
echo "renaming the latest to version name "$rename_to
mv mvBlueFOX-x86_64_ABI2-latest.tgz $rename_to
rm *.version