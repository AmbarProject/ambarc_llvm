# AmbarC Compiler

[![Build Status](https://github.com/AmbarProject/ambarc_llvm/actions/workflows/build-deb.yml/badge.svg)](https://github.com/AmbarProject/ambarc_llvm/actions)
[![Release](https://img.shields.io/github/v/release/AmbarProject/ambarc_llvm)](https://github.com/AmbarProject/ambarc_llvm/releases)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

AmbarC é uma linguagem de programação moderna e tipada estaticamente que compila para LLVM IR 18.

## 🚀 Instalação Rápida

### Ubuntu/Debian (Recomendado)
```bash
# Instalar com um comando
curl -fsSL https://raw.githubusercontent.com/AmbarProject/ambarc_llvm/main/install.sh | bash
```

### Ou manualmente:

```bash
# 1. Baixar o instalador
wget https://raw.githubusercontent.com/AmbarProject/ambarc_llvm/main/install.sh
chmod +x install.sh

# 2. Executar
sudo ./install.sh
```

### Pacote .deb

```bash
# Baixar o pacote mais recente
wget https://github.com/AmbarProject/ambarc_llvm/releases/latest/download/ambarc_1.0.0_amd64.deb

# Instalar
sudo dpkg -i ambarc_1.0.0_amd64.deb
sudo apt-get install -f
```

## 📦 O Que Está Incluído

- ambarc - Compilador completo (fonte → executável)
- ambarc-compile - Alias para compatibilidade
- Exemplos em /usr/local/share/ambarc/examples/
- Configuração automática do PATH

