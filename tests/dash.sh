#!/bin/bash

N_RUNS=1000 # número de repetições

average_time_and_mem() {
  local program=$1
  local sum_time=0
  local sum_mem=0

  for ((i = 1; i <= N_RUNS; i++)); do
    # Usar /usr/bin/time para capturar memória máxima
    # Memória vem no stderr, tempo real é calculado com date
    start=$(date +%s.%N)
    mem=$(/usr/bin/time -f "%M" $program 2>&1 >/dev/null)
    end=$(date +%s.%N)
    elapsed=$(echo "$end - $start" | bc -l)

    # Somar tempos e memória
    sum_time=$(echo "$sum_time + $elapsed" | bc -l)
    sum_mem=$(echo "$sum_mem + $mem" | bc -l)
  done

  avg_time=$(echo "scale=12; $sum_time / $N_RUNS" | bc -l)
  avg_mem=$(echo "scale=2; $sum_mem / $N_RUNS" | bc -l)

  echo "$avg_time $avg_mem"
}

# =========================
# Medir Ambar
# =========================
echo "Ambar: "
read ambar_time ambar_mem <<<$(average_time_and_mem "../output")
echo "Tempo médio: $ambar_time s"
echo "Memória média: $ambar_mem KB"

# =========================
# Medir C
# =========================
echo "C: "
read c_time c_mem <<<$(average_time_and_mem "./pi")
echo "Tempo médio: $c_time s"
echo "Memória média: $c_mem KB"
