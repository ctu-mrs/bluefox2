#!/bin/bash

curl  -O -J http://mrs.felk.cvut.cz/data/bluefox/mvBlueFOX-ARMhf_gnueabi-latest.tgz.version
rename_to=`cat mvBlueFOX-ARMhf_gnueabi-latest.tgz.version`
curl -O -J http://mrs.felk.cvut.cz/data/bluefox/mvBlueFOX-ARMhf_gnueabi-latest.tgz 
echo "renaming the latest to version name "$rename_to
mv mvBlueFOX-x86_64_ABI2-latest.tgz $rename_to
# curl  -O -J https://www.matrix-vision.com/USB2.0-single-board-camera-mvbluefox-mlc.html?file=tl_files/mv11/support/mvIMPACT_Acquire/01/mvBlueFOX-ARMhf_gnueabi-2.31.1.tgz
