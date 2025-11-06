@echo off
setlocal enabledelayedexpansion

echo 🚀 Instalador Ambar Compiler
echo ==========================================

set VERSION=1.0.0
set REPO_URL=https://github.com/seu-usuario/ambar-compiler

:: Detectar arquitetura
if "%PROCESSOR_ARCHITECTURE%"=="AMD64" (
    set ARCH=x64
) else (
    set ARCH=x86
)

set PLATFORM=windows-%ARCH%
set BINARY_NAME=ambar-%PLATFORM%.exe
set DOWNLOAD_URL=%REPO_URL%/releases/download/v%VERSION%/%BINARY_NAME%

:: Diretório de instalação
set INSTALL_DIR=%USERPROFILE%\AppData\Local\ambar-compiler\bin
set TARGET_PATH=%INSTALL_DIR%\ambar.exe

:: Criar diretório
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"

:: Download
echo 📦 Baixando Ambar Compiler...
powershell -Command "Invoke-WebRequest -Uri '%DOWNLOAD_URL%' -OutFile '%TARGET_PATH%'"

if errorlevel 1 (
    echo ❌ Erro no download
    exit /b 1
)

:: Adicionar ao PATH
set KEY=HKCU\Environment
for /f "usebackq tokens=2*" %%A in (`reg query "!KEY!" /v PATH 2^>nul`) do set CURRENT_PATH=%%B

echo !CURRENT_PATH! | find /i "!INSTALL_DIR!" > nul
if errorlevel 1 (
    set NEW_PATH=!CURRENT_PATH!;!INSTALL_DIR!
    reg add "!KEY!" /v PATH /d "!NEW_PATH!" /f > nul
    echo 🔧 Adicionado ao PATH do usuário
)

echo ✅ Ambar Compiler instalado com sucesso!
echo 📝 Use: ambar -O2 arquivo.amb
echo 📍 Instalado em: %TARGET_PATH%

:: Testar
if exist "%TARGET_PATH%" (
    echo.
    echo ℹ️  Verificando instalação:
    "%TARGET_PATH%" --version
)

echo.
echo 🎉 Pronto! Reinicie o terminal e comece a compilar.
pause
