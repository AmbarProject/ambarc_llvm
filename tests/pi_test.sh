#!/bin/bash

N_RUNS=100 # número de repetições (1000 pode ficar muito pesado com CPU e CSV)

# Função para medir tempo médio
average_time() {
  local program=$1
  local sum_time=0

  for ((i = 1; i <= N_RUNS; i++)); do
    start=$(date +%s.%N)
    $program >/dev/null 2>&1
    end=$(date +%s.%N)

    elapsed=$(echo "$end - $start" | bc -l)
    sum_time=$(echo "$sum_time + $elapsed" | bc -l)
  done

  avg_time=$(echo "scale=6; $sum_time / $N_RUNS" | bc -l)
  echo "$avg_time"
}

# =========================
# Arquivo de saída CSV
# =========================
CSV_FILE="metrics.csv"
echo "Linguagem,Tempo Médio (s)" >$CSV_FILE

# =========================
# Medir Ambar
# =========================
echo "🔹 Ambar:"
ambar_time=$(average_time "../ambar_files/pi")
echo "Tempo médio: $ambar_time s"
echo "Ambar,$ambar_time" >>$CSV_FILE
echo "⏳ Aguardando 10s para estabilizar..."
sleep 10

# =========================
# Medir C
# =========================
echo "🔹 C:"
c_time=$(average_time "../c_files/pi")
echo "Tempo médio: $c_time s"
echo "C,$c_time" >>$CSV_FILE
echo "⏳ Aguardando 10s para estabilizar..."
sleep 10

# =========================
# Medir Rust
# =========================
echo "🔹 Rust:"
rust_time=$(average_time "../rust_files/pi_file")
echo "Tempo médio: $rust_time s"
echo "Rust,$rust_time" >>$CSV_FILE

echo "✅ Métricas salvas em $CSV_FILE"
