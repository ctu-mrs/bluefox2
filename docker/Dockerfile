ARG PPA_VARIANT=stable

FROM ctumrs/mrs_uav_system:${PPA_VARIANT}

RUN apt-get -y update && apt-get -y install ros-jazzy-bluefox2 && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

COPY ros_entrypoint.sh /

CMD ["/ros_entrypoint.sh"]
