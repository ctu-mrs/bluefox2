#!/bin/bash
set -e

distro=`lsb_release -r | awk '{ print $2 }'`
[ "$distro" = "18.04" ] && ROS_DISTRO="melodic"
[ "$distro" = "20.04" ] && ROS_DISTRO="noetic"

echo "Starting install"

MY_PATH=`pwd`

sudo apt-get -y update -qq
sudo apt-mark hold openssh-server # the installation might get stuck while upgrading this
sudo apt-mark hold msodbcsql17 mssql-tools # microsoft wants to manually accept EULA while upgrading this

# 20.04 problem fix
sudo apt-get -y install grub-efi
sudo update-grub

# the "gce-compute-image-packages" package often freezes the installation at some point
# the installation freezes when it tries to manage some systemd services
# this attempts to install the package and stop the problematic service during the process
((sleep 90 && (sudo systemctl stop google-instance-setup.service && echo "gce service stoped" || echo "gce service not stoped")) & (sudo timeout 120s apt-get -y install gce-compute-image-packages)) || echo "\e[1;31mInstallation of gce-compute-image-packages failed\e[0m"

sudo apt-get -y upgrade --fix-missing

sudo apt-get -y install dpkg git

echo "clone uav_core"
cd
git clone https://github.com/ctu-mrs/uav_core.git
cd uav_core

echo "running the main install.sh"
./installation/install.sh

echo "clone camera_base"
cd
git clone https://github.com/ctu-mrs/camera_base.git

echo "installing bluefox2"
cd $MY_PATH/install
./install.sh

mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
ln -s ~/camera_base camera_base
ln -s "$MY_PATH" bluefox2
source /opt/ros/$ROS_DISTRO/setup.bash
cd ~/catkin_ws

echo "install ended"
