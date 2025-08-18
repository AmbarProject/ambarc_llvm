# Ambar Compiler (ambarc)

Um compilador moderno para a linguagem Ambar, construído com C++17, Bison, Flex e LLVM.

## 📋 Pré-requisitos

### Dependências Principais
- **C++ Compiler** (GCC 7+ ou Clang 6+)
- **Bison** 3.0+
- **Flex** 2.6+
- **LLVM** 10.0+
- **CMake** 3.15+ (opcional, para build automatizado)

---

## 🛠️ Instalação das Ferramentas

### 🐧 Linux (Ubuntu/Debian)

```bash
# Atualizar repositórios
sudo apt update

# Instalar ferramentas de build
sudo apt install build-essential cmake git

# Instalar Bison e Flex
sudo apt install bison flex

# Instalar LLVM
sudo apt install llvm-dev llvm-runtime llvm

# Verificar versões instaladas
gcc --version
bison --version
flex --version
llvm-config --version
```

### 🐧 Linux (CentOS/RHEL/Fedora)

```bash
# CentOS/RHEL
sudo yum groupinstall "Development Tools"
sudo yum install bison flex llvm-devel

# Fedora
sudo dnf groupinstall "Development Tools"
sudo dnf install bison flex llvm-devel cmake
```

### 🍎 macOS

#### Usando Homebrew (Recomendado)
```bash
# Instalar Homebrew (se não tiver)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Instalar dependências
brew install cmake git
brew install bison flex
brew install llvm

# Adicionar ao PATH (adicione ao seu .zshrc ou .bash_profile)
export PATH="/opt/homebrew/opt/bison/bin:$PATH"
export PATH="/opt/homebrew/opt/flex/bin:$PATH"
export PATH="/opt/homebrew/opt/llvm/bin:$PATH"
```

#### Usando MacPorts
```bash
sudo port install bison flex llvm-10 cmake
```

### 🪟 Windows

#### Opção 1: WSL2 (Recomendado)
```bash
# Instalar WSL2
wsl --install

# Seguir instruções do Linux Ubuntu acima
```

#### Opção 2: MSYS2/MinGW-w64
```bash
# Instalar MSYS2 de https://www.msys2.org/

# No terminal MSYS2
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-bison
pacman -S mingw-w64-x86_64-flex
pacman -S mingw-w64-x86_64-llvm
```

#### Opção 3: Visual Studio + vcpkg
```cmd
# Instalar vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Instalar dependências
.\vcpkg install llvm:x64-windows
.\vcpkg install flex-bison:x64-windows
```
---

## 📚 Recursos Adicionais

- **Documentação LLVM**: https://llvm.org/docs/
- **Manual Bison**: https://www.gnu.org/software/bison/manual/
- **Manual Flex**: https://github.com/westes/flex
- **Exemplos**: Veja a pasta `examples/` para programas de exemplo

---

## 🤝 Contribuindo

1. Fork o projeto
2. Crie uma branch para sua feature (`git checkout -b feature/AmazingFeature`)
3. Commit suas mudanças (`git commit -m 'Add some AmazingFeature'`)
4. Push para a branch (`git push origin feature/AmazingFeature`)
5. Abra um Pull Request
