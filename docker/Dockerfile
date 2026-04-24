ARG PPA_VARIANT=stable

FROM ctumrs/mrs_uav_system:${PPA_VARIANT}

COPY ros_entrypoint.sh /

RUN apt-get update && apt-get -y install ros-jazzy-bluefox2 && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

CMD ["/ros_entrypoint.sh"]
