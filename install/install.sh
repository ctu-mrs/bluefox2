#!/bin/bash

# get the path to this script
MY_PATH=`dirname "$0"`
MY_PATH=`( cd "$MY_PATH" && pwd )`

DOWNLOAD_LINK="http://static.matrix-vision.com/mvIMPACT_Acquire"
VERSION="2.34.0"

# install BLUEFOX
UNAME_PROC=`uname -m`
if [ "$UNAME_PROC" == "armv7l" ] ; then
  echo "installing bluefox for armv7l architecture"
  cd $MY_PATH/armv7l/
  ./download_driver.sh $DOWNLOAD_LINK $VERSION
  ./install_mvBlueFOX_ARM.sh --minimal
elif [ "$UNAME_PROC" == "aarch64" ] ; then
  echo "installing bluefox for aarch64 architecture"
  cd $MY_PATH/aarch64/
  ./download_driver.sh $DOWNLOAD_LINK $VERSION
  ./install_mvBlueFOX_AARCH64.sh --minimal
  cd /opt/mvIMPACT_acquire/lib
  ln -s ./arm64 aarch64
else
  echo "installing bluefox for x86_64 architecture"
  cd $MY_PATH/x86_64/
  ./download_driver.sh $DOWNLOAD_LINK $VERSION
  ./install_mvBlueFOX.sh --minimal
fi
