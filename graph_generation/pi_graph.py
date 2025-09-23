import subprocess
import time
import pandas as pd
import matplotlib.pyplot as plt

CSV_FILE = "../tests/metrics.csv"
SCRIPT = "../tests/pi_test.sh"

def run_bash_script():
    """Executa o pi_teste.sh para atualizar métricas"""
    print("🔄 Executando script de métricas...")
    subprocess.run([SCRIPT], check=True)

def load_metrics():
    """Carrega o CSV com os dados"""
    return pd.read_csv(CSV_FILE)

def plot_metrics(df):
    """Gera gráficos comparativos"""
    plt.figure(figsize=(12, 6))

    # Tempo
    plt.subplot(1, 3, 1)
    plt.bar(df["Linguagem"], df["Tempo Médio (s)"], color="skyblue")
    plt.title("Tempo Médio (s)")
    plt.ylabel("Segundos")

    # Memória
    plt.subplot(1, 3, 2)
    plt.bar(df["Linguagem"], df["Memória Média (KB)"], color="lightgreen")
    plt.title("Memória Média (KB)")
    plt.ylabel("KB")

    # CPU
    plt.subplot(1, 3, 3)
    plt.bar(df["Linguagem"], df["CPU Média (%)"], color="salmon")
    plt.title("CPU Média (%)")
    plt.ylabel("%")

    plt.suptitle("📊 Comparativo de Desempenho - PI", fontsize=16)
    plt.tight_layout(rect=[0, 0, 1, 0.95])
    plt.show(block=False)
    plt.pause(2)  # Mantém gráfico aberto por alguns segundos
    plt.clf()     # Limpa para próxima atualização

def main():
    
    run_bash_script()      # Atualiza métricas
    df = load_metrics()    # Lê CSV
    plot_metrics(df)       # Gera gráficos
    print(f"✅ Atualização concluída!")

if __name__ == "__main__":
    main()

