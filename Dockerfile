FROM ubuntu:24.04

LABEL maintainer="K1Melo <kauafernandes.smelo@gmail.com>"
LABEL version="1.0"

RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    flex \
    bison \
    clang \
    lld \
    zlib1g-dev \
    bc \
    file \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /home/developer/ambar

ENTRYPOINT ["./compile.sh"]
CMD ["--help"]
