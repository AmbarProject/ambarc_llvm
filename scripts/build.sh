#!/bin/bash

set -e

echo "🔨 Building Ambar Compiler in Docker..."

# Verificar se o Docker está instalado
if ! command -v docker &> /dev/null; then
    echo "❌ Docker não está instalado!"
    exit 1
fi

# Build da imagem
docker build -t ambar-compiler .

if [ $? -eq 0 ]; then
    echo "✅ Build completed successfully!"
    echo "🚀 To run: docker run -it ambar-compiler"
    echo "💻 For development: docker-compose run ambar-dev"
else
    echo "❌ Build failed!"
    exit 1
fi