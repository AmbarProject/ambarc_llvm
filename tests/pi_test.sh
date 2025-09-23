#!/bin/bash

N_RUNS=10 # número de repetições (1000 pode ficar muito pesado com CPU e CSV)

# Função para medir tempo, memória e CPU
average_metrics() {
  local program=$1
  local sum_time=0
  local sum_mem=0
  local sum_cpu=0

  for ((i = 1; i <= N_RUNS; i++)); do
    # Usar /usr/bin/time para capturar memória e CPU (%P)
    start=$(date +%s.%N)
    stats=$(/usr/bin/time -f "%M %P" $program 2>&1 >/dev/null)
    end=$(date +%s.%N)

    elapsed=$(echo "$end - $start" | bc -l)

    mem=$(echo "$stats" | awk '{print $1}')
    cpu=$(echo "$stats" | awk '{print $2}' | tr -d '%')

    # Somar métricas
    sum_time=$(echo "$sum_time + $elapsed" | bc -l)
    sum_mem=$(echo "$sum_mem + $mem" | bc -l)
    sum_cpu=$(echo "$sum_cpu + $cpu" | bc -l)
  done

  avg_time=$(echo "scale=6; $sum_time / $N_RUNS" | bc -l)
  avg_mem=$(echo "scale=2; $sum_mem / $N_RUNS" | bc -l)
  avg_cpu=$(echo "scale=2; $sum_cpu / $N_RUNS" | bc -l)

  echo "$avg_time $avg_mem $avg_cpu"
}

# =========================
# Arquivo de saída CSV
# =========================
CSV_FILE="metrics.csv"
echo "Linguagem,Tempo Médio (s),Memória Média (KB),CPU Média (%)" >$CSV_FILE

# =========================
# Medir Ambar
# =========================
echo "🔹 Ambar:"
read ambar_time ambar_mem ambar_cpu <<<$(average_metrics "../ambar_files/pi")
echo "Tempo médio: $ambar_time s"
echo "Memória média: $ambar_mem KB"
echo "CPU média: $ambar_cpu %"
echo "Ambar,$ambar_time,$ambar_mem,$ambar_cpu" >>$CSV_FILE
echo "⏳ Aguardando 30s para estabilizar..."
sleep 30

# =========================
# Medir C
# =========================
echo "🔹 C:"
read c_time c_mem c_cpu <<<$(average_metrics "../c_files/pi")
echo "Tempo médio: $c_time s"
echo "Memória média: $c_mem KB"
echo "CPU média: $c_cpu %"
echo "C,$c_time,$c_mem,$c_cpu" >>$CSV_FILE
echo "⏳ Aguardando 30s para estabilizar..."
sleep 30

# =========================
# Medir Rust
# =========================
echo "🔹 Rust:"
read rust_time rust_mem rust_cpu <<<$(average_metrics "../rust_files/pi_file")
echo "Tempo médio: $rust_time s"
echo "Memória média: $rust_mem KB"
echo "CPU média: $rust_cpu %"
echo "Rust,$rust_time,$rust_mem,$rust_cpu" >>$CSV_FILE
echo "⏳ Aguardando 30s para estabilizar..."
sleep 30

echo "✅ Métricas salvas em $CSV_FILE"
