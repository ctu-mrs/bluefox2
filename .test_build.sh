sudo apt-get update
sudo apt-get upgrade

USER_REPO_PATH=`pwd`
MY_WORKSPACE=~/workspace

git submodule update --init --recursive

rm -rf $MY_WORKSPACE
mkdir -p $MY_WORKSPACE/src

ln -s $USER_REPO_PATH $MY_WORKSPACE/src/user_repo
cd $MY_WORKSPACE
catkin init
catkin config --extend /opt/ros/kinetic
catkin config --profile default --cmake-args -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
catkin config --profile release --cmake-args -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
catkin profile set default
catkin build
