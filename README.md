# 🧠 Ambar Compiler — Ambiente Docker

Este projeto fornece um ambiente completo e isolado via **Docker** para compilar e executar o compilador **Ambar**, garantindo compatibilidade entre sistemas operacionais.

---

## 🚀 Requisitos

- **Docker** instalado (versão 24 ou superior)
- **Git** instalado

---

## 🧩 Instalação do Docker

### 🐧 Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install -y ca-certificates curl gnupg lsb-release

# Adiciona o repositório oficial do Docker
sudo mkdir -p /etc/apt/keyrings
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg

echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] \
  https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

# Instala o Docker
sudo apt update
sudo apt install -y docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

# Verifica se está funcionando
sudo docker --version
````

---

### 🍎 MacOS (via Terminal)

```bash
# Instala o Homebrew se ainda não tiver
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Instala o Docker Desktop
brew install --cask docker

# Inicie o Docker Desktop manualmente (necessário na primeira execução)
open /Applications/Docker.app
```

---

### 🪟 Windows (via PowerShell)

Execute como **Administrador**:

```powershell
# Baixa e instala o Docker Desktop
Invoke-WebRequest -UseBasicParsing -Uri "https://desktop.docker.com/win/main/amd64/Docker%20Desktop%20Installer.exe" -OutFile "DockerInstaller.exe"
Start-Process -Wait -FilePath ".\DockerInstaller.exe" -ArgumentList "install", "--quiet"

# Reinicie o sistema após a instalação
Restart-Computer
```

Após reiniciar, abra o **Docker Desktop** e certifique-se de que ele esteja rodando.

---

## 🧬 Clonando o Projeto

```bash
git clone https://github.com/AmbarProject/ambarc_llvm.git
cd ambarc_llvm
```

---

## 🧱 Build da Imagem Docker

```bash
docker build -t ambar:latest .
```

---

## 🐋 Executando o Container

```bash
# Linux / macOs
docker run -it --rm -v $(pwd):/workspace ambar:latest

# Windows
docker run -it --rm -v "%cd%":/workspace ambar:latest

```

> 🔹 Isso monta o diretório atual (`$(pwd)`) dentro do container no caminho `/workspace`, permitindo editar os arquivos localmente e compilar dentro do Docker.

---

## ⚙️ Compilando com o Ambar

Dentro do container, rode:

```bash
./compile.sh
```

> Esse script compila o código-fonte Ambar e gera o arquivo LLVM (`.ll`) correspondente.

---

## 🧾 Estrutura de Pastas

```
├── src/                # Código-fonte principal do compilador
├── include/            # Cabeçalhos (headers)
├── Dockerfile          # Configuração do ambiente Docker
├── compile.sh          # Script de compilação
├── ir.sh               # Geração de código LLVM IR
└── test.amb            # Código de teste para o compilador
```

---

## 🧠 Dicas Úteis

* Para sair do container:

  ```bash
  exit
  ```
* Para rebuildar completamente a imagem (limpando cache):

  ```bash
  docker build --no-cache -t ambar:latest .
  ```
* Para acessar o shell do container:

  ```bash
  docker run -it ambar:latest /bin/bash
  ```

---

## 📜 Licença

Este projeto é distribuído sob a licença **MIT**.
