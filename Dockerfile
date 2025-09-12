FROM silkeh/clang:16

# Metadados
LABEL maintainer="Seu Nome <seu.email@example.com>"
LABEL version="1.0"

# Instalar dependências básicas
RUN apt-get update && apt-get install -y \
    make \
    git \
    zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

# Criar usuário não-root
RUN useradd -m -s /bin/bash developer
USER developer
WORKDIR /home/developer

COPY --chown=developer:developer . /home/developer/ambar
WORKDIR /home/developer/ambar

# Build manual SEM CMake - abordagem direta
RUN mkdir -p build && \
    cd build && \
    # Compilar todos os arquivos .cpp
    find ../src -name "*.cpp" -exec clang++ -std=c++17 -I../include {} -c $(llvm-config-16 --cxxflags) \; && \
    # Linkar tudo
    clang++ *.o -o ambar_compiler $(llvm-config-16 --ldflags) $(llvm-config-16 --libs core support) $(llvm-config-16 --system-libs)

ENTRYPOINT ["./build/ambar_compiler"]
CMD ["--help"]