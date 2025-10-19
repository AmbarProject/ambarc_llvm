import pandas as pd
import matplotlib.pyplot as plt

# Ler CSV
df = pd.read_csv("metrics.csv")

# Normalizar em relação ao tempo de C
c_time = df.loc[df["Linguagem"] == "C", "Tempo_Medio_s"].values[0]
df["Velocidade_Relativa"] = df["Tempo_Medio_s"] / c_time

# Criar gráfico de barras normalizado
plt.figure(figsize=(8, 5))
bars = plt.bar(df["Linguagem"], df["Velocidade_Relativa"], color=["#2930ff", "#01010f", "#f4f736", "#ed7d2d","#2d90ed","#ed2d2d"])
plt.xlabel("Linguagem")
plt.ylabel("Tempo relativo (C = 1x)")
plt.title("Comparação de Velocidade Relativa de Execução")
plt.grid(axis="y", linestyle="--", alpha=0.7)

# Mostrar valores no topo das barras
for bar in bars:
    height = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2.0, height + 0.01, f"{height:.2f}x", ha='center', va='bottom', fontsize=10)

plt.tight_layout()
plt.savefig("metrics_relative.png")  # salva como imagem
plt.show()
