FROM ubuntu:22.04

# Evita prompts interativos durante a instalação
ENV DEBIAN_FRONTEND=noninteractive

# Instala dependências básicas do sistema e ferramentas de build
RUN apt-get update && apt-get install -y \
    build-essential \
    wget \
    curl \
    gnupg \
    lsb-release \
    software-properties-common \
    flex \
    bison \
    gcc \
    g++ \
    make \
    bash \
    vim \
    git \
    lld && \
    rm -rf /var/lib/apt/lists/*

# Instala LLVM 17 e Clang 17 do repositório oficial apt.llvm.org
RUN wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh && \
    ./llvm.sh 17 && \
    rm llvm.sh

# Define variáveis de ambiente para LLVM 17
ENV LLVM_DIR=/usr/lib/llvm-17
ENV PATH=$LLVM_DIR/bin:$PATH
ENV LD_LIBRARY_PATH=$LLVM_DIR/lib:$LD_LIBRARY_PATH

# Cria diretório de trabalho padrão
WORKDIR /workspace

# Copia o script de compilação para o container
COPY compile.sh /workspace/compile.sh
RUN chmod +x /workspace/compile.sh

# Mantém o container ocioso no workspace
CMD ["bash"]

