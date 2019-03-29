#!/bin/bash
# author: Tomas Baca

# install BLUEFOX
UNAME_PROC=`uname -m`
if [ "$UNAME_PROC" == "armv7l" ] ; then
  echo "installing bluefox for armv71 architecture"
  cd ~/git/uav_modules/ros_packages/bluefox2/install/armv7l/
  ./download_driver.sh
  ./install_mvBlueFOX_ARM.sh --unattended
elif [ "$UNAME_PROC" == "aarch64" ] ; then
  echo "installing bluefox for aarch64 architecture"
  cd ~/git/uav_modules/ros_packages/bluefox2/install/aarch64/
  ./download_driver.sh
  ./install_mvBlueFOX_AARCH64.sh --unattended
  cd /opt/mvIMPACT_acquire/lib
  ln -s ./arm64 aarch64
else
  echo "installing bluefox for x86_64 architecture"
  cd ~/git/uav_modules/ros_packages/bluefox2/install/x86_64/
  ./download_driver.sh
  ./install_mvBlueFOX.sh --unattended
fi
