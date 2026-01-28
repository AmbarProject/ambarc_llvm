
---

# AmbarC Compiler

[![Build Status](https://github.com/AmbarProject/ambarc_llvm/actions/workflows/build-deb.yml/badge.svg)](https://github.com/AmbarProject/ambarc_llvm/actions)
[![Release](https://img.shields.io/github/v/release/AmbarProject/ambarc_llvm)](https://github.com/AmbarProject/ambarc_llvm/releases)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

**AmbarC** é uma linguagem de programação moderna, tipada estaticamente, que compila para **LLVM IR 18**, com foco em eficiência, desempenho e portabilidade.

---

## 🚀 Instalação

A instalação varia de acordo com o sistema operacional.
Escolha a seção correspondente ao seu ambiente.

---

## 🐧 Instalação no Linux (Ubuntu/Debian)

> ✅ Recomendado para desenvolvimento nativo

### Requisitos

* Ubuntu 20.04+ ou Debian 11+
* `sudo` habilitado
* Conexão com a internet

---

### 1️⃣ Baixar o instalador

```bash
wget https://raw.githubusercontent.com/AmbarProject/ambarc_llvm/main/install.sh
chmod +x install.sh
```

---

### 2️⃣ Executar o instalador

```bash
sudo ./install.sh
```

O script irá:

* Instalar dependências automaticamente
* Compilar o projeto
* Instalar o compilador no sistema
* Configurar o PATH

---

### 3️⃣ Verificar a instalação

```bash
ambarc --version
```

Se o comando responder corretamente, o AmbarC está pronto para uso 🎉

---

## 🪟 Instalação no Windows (via WSL)

O AmbarC é desenvolvido para ambientes Linux.
No Windows, o método oficial e recomendado é através do **WSL (Windows Subsystem for Linux)**.

---

### ✅ Requisitos

* Windows 10 (versão 2004+) ou Windows 11
* Permissões de administrador

---

### 1️⃣ Instalar o WSL

Abra o **PowerShell como Administrador** e execute:

```powershell
wsl --install
```

Esse comando irá:

* Instalar o WSL
* Instalar o Ubuntu como distribuição padrão
* Configurar o ambiente automaticamente

> 🔁 Reinicie o computador se solicitado.

---

### 2️⃣ Abrir o Ubuntu (WSL)

Após a instalação, abra o **Ubuntu** pelo menu iniciar
ou execute no PowerShell:

```powershell
wsl
```

Na primeira execução, será solicitado:

* Nome de usuário Linux
* Senha

---

### 3️⃣ Atualizar o sistema Linux

No terminal do Ubuntu (WSL):

```bash
sudo apt update && sudo apt upgrade -y
```

---

### 4️⃣ Instalar o AmbarC no WSL

Siga o mesmo processo de instalação Linux:

```bash
wget https://raw.githubusercontent.com/AmbarProject/ambarc_llvm/main/install.sh
chmod +x install.sh
sudo ./install.sh
```

---

### 5️⃣ Verificar a instalação

```bash
ambarc --version
```

Se o comando funcionar, o compilador está corretamente instalado 🚀

---

### 💡 Dicas para usuários Windows

* Arquivos do Windows ficam disponíveis em `/mnt/c`
* Recomenda-se usar **VS Code com WSL**
* O desempenho de compilação é próximo ao nativo

---

## 📦 O Que Está Incluído

* `ambarc` — compilador completo (fonte → executável)
* `ambarc-compile` — alias para compatibilidade
* Exemplos em `/usr/local/share/ambarc/examples/`
* Configuração automática do `PATH`

---
