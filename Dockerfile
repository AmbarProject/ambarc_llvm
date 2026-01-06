# Base
FROM ubuntu:22.04

# Evita prompts interativos durante a instalação
ENV DEBIAN_FRONTEND=noninteractive

# Atualiza o sistema e instala dependências básicas
RUN apt-get update && apt-get install -y \
    software-properties-common \
    wget \
    curl \
    gnupg \
    lsb-release \
    build-essential \
    flex \
    bison \
    gcc \
    g++ \
    make \
    bash \
    vim \
    git \
    lld \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Adiciona PPA para obter libstdc++ atualizada (GCC 13)
RUN add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
    apt-get update && \
    apt-get install -y libstdc++6 && \
    rm -rf /var/lib/apt/lists/*

# Instala LLVM 18.1.3 via llvm.sh
RUN wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh && \
    ./llvm.sh 18 && \
    rm llvm.sh

# Define variáveis de ambiente para LLVM 18
ENV LLVM_DIR=/usr/lib/llvm-18
ENV PATH=$LLVM_DIR/bin:$PATH
ENV LD_LIBRARY_PATH=$LLVM_DIR/lib:$LD_LIBRARY_PATH

# Diretório de trabalho
WORKDIR /workspace

COPY ./compiler/compile.sh /workspace/compiler/compile.sh
RUN chmod +x /workspace/compiler/compile.sh

CMD ["bash"]

