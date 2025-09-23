#!/bin/bash

# Este script mede o tempo de execução de um programa.
# Use: ./nome_do_script.sh <caminho_para_o_programa>

# Verifica se um arquivo foi passado como argumento
if [ -z "$1" ]; then
  echo "Erro: Você precisa fornecer o caminho para o arquivo executável."
  echo "Uso: $0 <caminho_para_o_programa>"
  exit 1
fi

# Verifica se o arquivo existe e é executável
if [ ! -x "$1" ]; then
  echo "Erro: O arquivo '$1' não existe ou não é executável."
  exit 1
fi

# Medir tempo com 12 casas decimais
start=$(date +%s.%N)

# Executa o programa passado como argumento
"$1"

end=$(date +%s.%N)
duration=$(echo "$end - $start" | bc -l)

printf "Tempo: %.12f segundos\n" $duration
