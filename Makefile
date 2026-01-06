.PHONY: all build clean install uninstall deb test help

VERSION=1.0.0
ARCH=$(shell dpkg --print-architecture)

help:
	@echo "AmbarC - Sistema de Build"
	@echo ""
	@echo "Comandos disponíveis:"
	@echo "  make build      - Compilar o compilador"
	@echo "  make clean      - Limpar arquivos temporários"
	@echo "  make deb        - Criar pacote .deb"
	@echo "  make install    - Instalar localmente"
	@echo "  make uninstall  - Remover instalação"
	@echo "  make test       - Testar instalação"
	@echo "  make all        - Build completo + pacote"
	@echo ""

all: build deb

build:
	@echo "🔨 Compilando AmbarC..."
	@cd compiler && ./rm.sh && ./ir.sh
	@echo "✅ Compilação concluída!"

clean:
	@echo "🧹 Limpando..."
	@cd compiler && ./rm.sh
	@rm -f ambarc_*.deb
	@echo "✅ Limpeza concluída!"

deb: build
	@echo "📦 Criando pacote .deb..."
	@./tools/create-deb.sh

install: deb
	@echo "📥 Instalando..."
	@sudo ./tools/installer.sh

uninstall:
	@echo "🗑️  Desinstalando AmbarC..."
	@sudo dpkg -r ambarc || true
	@sudo rm -f /etc/profile.d/ambarc.sh
	@echo "✅ AmbarC removido!"

test:
	@echo "🧪 Testando AmbarC..."
	@if command -v ambarc >/dev/null 2>&1; then \
		echo "✅ ambarc encontrado"; \
		ambarc --version; \
	else \
		echo "❌ ambarc não encontrado"; \
		exit 1; \
	fi

# Comando rápido para usuários
quick:
	@echo "⚡ Instalação rápida do AmbarC..."
	@sudo apt-get update
	@sudo apt-get install -y build-essential llvm-14 clang-14 bison flex
	@make build
	@sudo cp compiler/bin/ambarc /usr/local/bin/
	@sudo cp compiler/compile.sh /usr/local/bin/ambarc-compile
	@echo 'export PATH="/usr/local/bin:$$PATH"' | sudo tee /etc/profile.d/ambarc.sh
	@echo "✅ Instalação rápida concluída!"
	@echo "   Reinicie o terminal e execute: ambarc --version"