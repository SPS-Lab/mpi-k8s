FROM ubuntu:16.04
RUN  apt-get update --fix-missing \
     && apt-get install -y libopenmpi-dev openmpi-bin \
     && apt-get install -y git \
     && apt-get install -y build-essential \
     && apt-get install -y cmake \
     && apt-get install -y libopenblas-dev \
     && apt-get install -y ssh \
     && apt-get clean \
     && rm -rf /var/lib/apt/lists/*
RUN git clone https://github.com/SPS-Lab/mpi-k8s \
    && cd mpi-k8s/BDAS \
    && make \
    && cp build/princomp /home/ \
    && apt-get autoremove -y git \
    && apt-get autoremove -y build-essential \
    && apt-get autoremove -y cmake \
    && rm -rf "/mpi-k8s"
CMD mkdir -p /var/run/sshd; /usr/sbin/sshd;