ARG PPA_VARIANT=stable
ARG BASE_IMAGE=ctumrs/mrs_uav_system_core:${PPA_VARIANT}

FROM ${BASE_IMAGE}

COPY ros_entrypoint.sh /

RUN apt-get update && apt-get -y install ros-${ROS_DISTRO}-bluefox2 && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

CMD ["/ros_entrypoint.sh"]
