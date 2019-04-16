#!/bin/bash

# get the path to this script
MY_PATH=`dirname "$0"`
MY_PATH=`( cd "$MY_PATH" && pwd )`

# install BLUEFOX
UNAME_PROC=`uname -m`
if [ "$UNAME_PROC" == "armv7l" ] ; then
  echo "installing bluefox for armv71 architecture"
  cd $MY_PATH/arm/
  ./download_driver.sh
  ./install_mvBlueFOX_ARM.sh --unattended
else
  echo "installing bluefox for x86_64 architecture"
  cd $MY_PATH/x86_64/
  ./download_driver.sh
  ./install_mvBlueFOX.sh --unattended
fi
