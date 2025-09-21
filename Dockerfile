FROM ubuntu:22.04

# Metadados
LABEL maintainer="K1Melo <kauafernandes.smelo@gmail.com>"
LABEL version="1.0"

# Instalar dependências básicas
RUN apt-get update && apt-get install -y \
    gcc \
    g++ \
    make \
    git \
    flex \
    bison \
    llvm \
    clang \
    lld \
    zlib1g-dev \
    libllvm-dev \
    && rm -rf /var/lib/apt/lists/*

# Criar usuário não-root
RUN useradd -m -s /bin/bash developer
USER developer
WORKDIR /home/developer

COPY --chown=developer:developer . /home/developer/ambar
WORKDIR /home/developer/ambar

# Dar permissão de execução aos scripts
RUN chmod +x *.sh

# Verificar versões instaladas
RUN gcc --version && g++ --version && llc --version

# Definir o script compile.sh como entrypoint
ENTRYPOINT ["./compile.sh"]
