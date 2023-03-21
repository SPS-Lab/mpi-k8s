FROM nvidia/cuda:11.2.0-devel-ubuntu18.04
RUN mkdir -p /run/systemd && echo 'docker' > /run/systemd/container
ENV GPU_LIBRARY_PATH /usr/local/cuda/lib64
ENV GPU_INCLUDE_PATH /usr/local/cuda/include
RUN apt-get update && apt-get install -y git
RUN cd /home && git clone --depth 1 --branch v1.5.3 https://github.com/ccsb-scripps/AutoDock-GPU.git \
    && cd AutoDock-GPU \
    && make DEVICE=GPU NUMWI=64 \
    && cp bin/autodock_gpu_64wi /home/.
