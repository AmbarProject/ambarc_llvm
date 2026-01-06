#!/bin/bash
# Configuração do PATH para AmbarC
AMBARC_PATH="/usr/local/bin"

if [[ ":$PATH:" != *":$AMBARC_PATH:"* ]]; then
    export PATH="$PATH:$AMBARC_PATH"
fi

# Função helper (opcional)
ambarc-build() {
    if [ $# -eq 0 ]; then
        echo "Uso: ambarc-build <arquivo.amb>"
        echo "     (alias para: ambarc -O2 <arquivo.amb>)"
        return 1
    fi
    ambarc -O2 "$@"
}
