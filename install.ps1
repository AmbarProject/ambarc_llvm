# Instalador Ambar Compiler para Windows
Write-Host "🚀 Instalador Ambar Compiler" -ForegroundColor Green
Write-Host "==========================================" -ForegroundColor Yellow

$Version = "1.0.0"
$RepoUrl = "https://github.com/seu-usuario/ambar-compiler"

# Detectar arquitetura
if ([Environment]::Is64BitOperatingSystem) {
    $Arch = "x64"
} else {
    $Arch = "x86"
}

$Platform = "windows-$Arch"
$BinaryName = "ambar-$Platform.exe"
$DownloadUrl = "$RepoUrl/releases/download/v$Version/$BinaryName"

# Diretório de instalação
$InstallDir = "$env:USERPROFILE\AppData\Local\ambar-compiler\bin"
$TargetPath = "$InstallDir\ambar.exe"

# Criar diretório
if (!(Test-Path $InstallDir)) {
    New-Item -ItemType Directory -Path $InstallDir -Force
}

# Download
Write-Host "📦 Baixando Ambar Compiler..." -ForegroundColor Yellow
try {
    Invoke-WebRequest -Uri $DownloadUrl -OutFile $TargetPath
} catch {
    Write-Host "❌ Erro no download: $($_.Exception.Message)" -ForegroundColor Red
    exit 1
}

# Adicionar ao PATH do usuário
$UserPath = [Environment]::GetEnvironmentVariable("PATH", "User")
if ($UserPath -notlike "*$InstallDir*") {
    [Environment]::SetEnvironmentVariable("PATH", "$UserPath;$InstallDir", "User")
    Write-Host "🔧 Adicionado ao PATH do usuário" -ForegroundColor Green
}

Write-Host "✅ Ambar Compiler instalado com sucesso!" -ForegroundColor Green
Write-Host "📝 Use: ambar -O2 arquivo.amb" -ForegroundColor Yellow
Write-Host "📍 Instalado em: $TargetPath" -ForegroundColor Cyan

# Testar
if (Test-Path $TargetPath) {
    Write-Host "`nℹ️  Verificando instalação:" -ForegroundColor Cyan
    & $TargetPath --version
}

Write-Host "`n🎉 Pronto! Reinicie o terminal e comece a compilar." -ForegroundColor Green
