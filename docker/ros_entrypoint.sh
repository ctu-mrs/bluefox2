#!/bin/bash

. /opt/ros/jazzy/setup.sh

if [ -z "$UAV_NAME" ]; then
  echo "$0: the UAV_NAME environment variable is not set"
  #exit 1
fi

# Filter out the script name from arguments.
# If you do not provide any arguments, the $@ will contain name of the script "/ros_entrypoint.sh",
# which is invalid ROS argument and the node is not launched. If you provide some argument,
# then it is ok, because the name of the script will not be present in $@, only provided arguments.
LAUNCH_ARGS=""
for arg in "$@"; do
    if [[ "$arg" != *"ros_entrypoint.sh"* ]]; then
        LAUNCH_ARGS="$LAUNCH_ARGS $arg"
    fi
done

#echo "Launch arguments: $LAUNCH_ARGS"
ros2 launch bluefox2 single.launch.py $LAUNCH_ARGS