ARG BASE_IMAGE=ctumrs/mrs_uav_system:stable

FROM ${BASE_IMAGE}

RUN curl https://ctu-mrs.github.io/ppa2-stable/add_ppa.sh | bash

RUN apt-get -y update && apt-get -y install ros-jazzy-bluefox2  && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

COPY ros_entrypoint.sh /

CMD ["/ros_entrypoint.sh"]
