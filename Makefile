.PHONY: all build clean install uninstall test help quick fix-deb

VERSION=1.0.0

help:
	@echo "AmbarC - Sistema de Build"
	@echo ""
	@echo "Comandos disponíveis:"
	@echo "  make build      - Compilar o compilador"
	@echo "  make clean      - Limpar arquivos temporários"
	@echo "  make install    - Instalar localmente (sudo)"
	@echo "  make uninstall  - Remover instalação (sudo)"
	@echo "  make test       - Testar instalação"
	@echo "  make quick      - Instalação rápida"
	@echo "  make fix-deb    - Corrigir pacote .deb"
	@echo ""

all: build

build:
	@echo "🔨 Compilando AmbarC..."
	@cd compiler && ./rm.sh && ./ir.sh
	@echo "✅ Compilação concluída!"

clean:
	@echo "🧹 Limpando..."
	@cd compiler && ./rm.sh
	@rm -f ambarc_*.deb
	@rm -rf ambarc_*_amd64
	@echo "✅ Limpeza concluída!"

install:
	@echo "📥 Instalando AmbarC..."
	@sudo ./tools/simple-install.sh

uninstall:
	@echo "🗑️  Desinstalando AmbarC..."
	@sudo rm -f /usr/local/bin/ambarc
	@sudo rm -f /usr/local/bin/ambarc-compile
	@sudo rm -f /etc/profile.d/ambarc.sh
	@echo "✅ AmbarC removido!"
	@echo "   Reinicie o terminal para atualizar o PATH."

test:
	@echo "🧪 Testando AmbarC..."
	@if command -v ambarc >/dev/null 2>&1; then \
		echo "✅ ambarc encontrado"; \
		ambarc --version; \
	else \
		echo "❌ ambarc não encontrado"; \
		echo "   Execute: make install"; \
		exit 1; \
	fi

quick: build install

fix-deb:
	@echo "🔧 Corrigindo pacote .deb..."
	@./tools/fix-deb.sh