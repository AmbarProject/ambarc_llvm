#!/bin/bash
set -euo pipefail

N_RUNS=5 # número de repetições (ajuste conforme necessário)
RAPL_PATH="/sys/class/powercap/intel-rapl:0/energy_uj"
CSV_FILE="metrics_pi_energy.csv"

# Verifica se RAPL existe
if [ ! -r "$RAPL_PATH" ]; then
  echo "Aviso: RAPL ($RAPL_PATH) não acessível. Verifique se o kernel expõe RAPL ou rode com sudo." >&2
  # Não sai; ainda permitimos rodar (irá retornar zeros), mas avise.
fi

# =========================
# Função para medir tempo e energia
# =========================
measure_metrics() {
  local program="$1"
  local sum_time=0
  local sum_energy=0

  for ((i = 1; i <= N_RUNS; i++)); do
    # Logs de progresso para STDERR (assim aparecem no terminal mesmo quando capturados)
    echo "[$program] Execução $i/$N_RUNS ..." >&2

    # leitura inicial de energia (se possível)
    if [ -r "$RAPL_PATH" ]; then
      start_energy=$(sudo cat "$RAPL_PATH")
    else
      start_energy=0
    fi
    start_time=$(date +%s.%N)

    # Executa o programa (redireciona stdout/stderr para /dev/null)
    # se quiser ver saída do programa, remova os redirecionamentos
    $program >/dev/null 2>&1 || true

    end_time=$(date +%s.%N)
    if [ -r "$RAPL_PATH" ]; then
      end_energy=$(sudo cat "$RAPL_PATH")
      # RAPL em microjoules -> converter para Joules
      energy_used=$(echo "scale=9; ($end_energy - $start_energy) / 1000000" | bc -l)
    else
      energy_used=0
    fi

    elapsed_time=$(echo "$end_time - $start_time" | bc -l)

    # Log detalhado em STDERR
    echo "  → Tempo: ${elapsed_time}s | Energia: ${energy_used}J" >&2

    sum_time=$(echo "$sum_time + $elapsed_time" | bc -l)
    sum_energy=$(echo "$sum_energy + $energy_used" | bc -l)

    sleep 1
  done

  # calcula médias (proteção contra divisão por zero)
  if [ "$N_RUNS" -gt 0 ]; then
    avg_time=$(echo "scale=6; $sum_time / $N_RUNS" | bc -l)
    avg_energy=$(echo "scale=6; $sum_energy / $N_RUNS" | bc -l)
  else
    avg_time=0
    avg_energy=0
  fi

  # calcula potência média (se tempo > 0)
  if [ "$(echo "$avg_time > 0" | bc -l)" -eq 1 ]; then
    avg_power=$(echo "scale=6; $avg_energy / $avg_time" | bc -l)
  else
    avg_power=0
  fi

  # Retorna apenas a linha de resultado em stdout (campos separados por vírgula)
  echo "${avg_time},${avg_energy},${avg_power}"
}

# =========================
# Cabeçalho CSV
# =========================
echo "Linguagem,Tempo_Medio_s,Energia_Media_J,Potencia_Media_W" >"$CSV_FILE"

# Helper para chamar medida e gravar CSV (captura stdout da função e imprime logs no terminal)
run_and_record() {
  local label="$1"
  local cmd="$2"

  # Captura o resultado (stdout) da função; mensagens de progresso vão para stderr e aparecem no terminal.
  result=$(measure_metrics "$cmd")
  # Usa IFS para separar por vírgula
  IFS=',' read -r avg_t avg_e avg_p <<<"$result"

  # Segurança: garanta que variáveis não vazias
  avg_t=${avg_t:-0}
  avg_e=${avg_e:-0}
  avg_p=${avg_p:-0}

  echo "$label,$avg_t,$avg_e,$avg_p" >>"$CSV_FILE"
  echo "Registrado: $label -> tempo=$avg_t s, energia=${avg_e} J, potência=${avg_p} W" >&2
}

# =========================
# Chamadas (ajuste caminhos conforme necessário)
# =========================
echo "🔹 C:"
run_and_record "C" "../../c_files/pi"
sleep 2

echo "🔹 Rust:"
run_and_record "Rust" "../../rust_files/pi_file"
sleep 2

echo "🔹 Ambar:"
run_and_record "Ambar" "../../ambar_files/pi"
sleep 2

echo "🔹 JavaScript:"
run_and_record "JavaScript" "node ../../javascript_files/pi.js"
sleep 2

echo "🔹 Java:"
# Se necessário, compile aqui (exemplo comentado)
# javac ../../java_files/PiCalculator.java
run_and_record "Java" "java -cp ../../java_files PiCalculator"
sleep 2

echo "🔹 Python:"
run_and_record "Python" "python3 ../../python_files/pi.py"
sleep 2

echo "🔹 R:"
run_and_record "R" "Rscript ../../r_files/pi.R"
sleep 2

echo "✅ Métricas completas salvas em $CSV_FILE" >&2
