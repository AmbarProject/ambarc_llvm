#!/bin/bash

N_RUNS=1 # número de repetições (ajuste conforme necessário)

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
echo "Linguagem,Tempo_Medio_s" >$CSV_FILE

# =========================
# Medir C
# =========================
echo "🔹 C:"
c_time=$(average_time "../c_files/pi")
echo "Tempo médio: $c_time s"
echo "C,$c_time" >>$CSV_FILE
sleep 5

# =========================
# Medir Rust
# =========================
echo "🔹 Rust:"
rust_time=$(average_time "../rust_files/pi_file")
echo "Tempo médio: $rust_time s"
echo "Rust,$rust_time" >>$CSV_FILE
sleep 5

# =========================
# Medir Ambar
# =========================
echo "🔹 Ambar:"
ambar_time=$(average_time "../ambar_files/pi")
echo "Tempo médio: $ambar_time s"
echo "Ambar,$ambar_time" >>$CSV_FILE
sleep 5

# =========================
# Medir JavaScript
# =========================
echo "🔹 JavaScript:"
js_time=$(average_time "node ../javascript_files/pi.js")
echo "Tempo médio: $js_time s"
echo "JavaScript,$js_time" >>$CSV_FILE
sleep 5

# =========================
# Medir Java
# =========================
echo "🔹 Java:"
# Compilar Java primeiro
java_time=$(average_time "java -cp ../java_files PiCalculator")
echo "Tempo médio: $java_time s"
echo "Java,$java_time" >>$CSV_FILE
sleep 5

# =========================
# Medir Python
# =========================
echo "🔹 Python:"
python_time=$(average_time "python3 ../python_files/pi.py")
echo "Tempo médio: $python_time s"
echo "Python,$python_time" >>$CSV_FILE

echo "✅ Métricas salvas em $CSV_FILE"
