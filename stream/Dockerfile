FROM ubuntu:22.04
RUN /bin/sh -c set -xe \ 
    && echo '#!/bin/sh' > /usr/sbin/policy-rc.d \
    && echo 'exit 101' >> /usr/sbin/policy-rc.d \
    && chmod +x /usr/sbin/policy-rc.d \
    && dpkg-divert --local --rename --add /sbin/initctl \
    && cp -a /usr/sbin/policy-rc.d /sbin/initctl \
    && sed -i 's/^exit.*/exit 0/' /sbin/initctl \		
    && echo 'force-unsafe-io' > /etc/dpkg/dpkg.cfg.d/docker-apt-speedup \
    && echo 'DPkg::Post-Invoke { "rm -f /var/cache/apt/archives/*.deb /var/cache/apt/archives/partial/*.deb /var/cache/apt/*.bin || true"; };' > /etc/apt/apt.conf.d/docker-clean \
    && echo 'APT::Update::Post-Invoke { "rm -f /var/cache/apt/archives/*.deb /var/cache/apt/archives/partial/*.deb /var/cache/apt/*.bin || true"; };' >> /etc/apt/apt.conf.d/docker-clean \	
    && echo 'Dir::Cache::pkgcache ""; Dir::Cache::srcpkgcache "";' >> /etc/apt/apt.conf.d/docker-clean 	\
    && echo 'Acquire::Languages "none";' > /etc/apt/apt.conf.d/docker-no-languages 	\
    && echo 'Acquire::GzipIndexes "true"; Acquire::CompressionTypes::Order:: "gz";' > /etc/apt/apt.conf.d/docker-gzip-indexes \
    && echo 'Apt::AutoRemove::SuggestsImportant "false";' > /etc/apt/apt.conf.d/docker-autoremove-suggests
RUN rm -rf /var/lib/apt/lists/*
RUN mkdir -p /run/systemd && echo 'docker' > /run/systemd/container
RUN  apt-get update --fix-missing \
     && apt-get install -y libopenmpi-dev openmpi-bin \
     && apt-get install -y git \
     && apt-get install -y build-essential \
     && apt-get install -y cmake \
     && apt-get install -y libopenblas-dev \
     && apt-get install -y ssh \
     && apt-get clean \
     && rm -rf /var/lib/apt/lists/*
RUN git clone https://github.com/SPS-Lab/mpi-k8s.git \
    && cd mpi-k8s/stream \
    && make \
    && cp stream_c /home/ \
    && cp stream.c /home/ \
    && apt-get autoremove -y git \
    && apt-get autoremove -y build-essential \
    && apt-get autoremove -y cmake \
    && rm -rf "/mpi-k8s"
CMD mkdir -p /var/run/sshd; /usr/sbin/sshd;
