import pandas as pd
import matplotlib.pyplot as plt
import os

# =====================
# Carregar dados
# =====================
df = pd.read_csv("execucoes.csv")
df.columns = [c.strip() for c in df.columns]

# =====================
# Normalização Min-Max (0% - 100%)
# =====================
cpu_min = df["CPU (%)"].min()
cpu_max = df["CPU (%)"].max()
df["CPU_norm"] = (df["CPU (%)"] - cpu_min) / (cpu_max - cpu_min) * 100

# =====================
# Criar pasta results
# =====================
os.makedirs("results", exist_ok=True)

# =====================
# Gráfico geral (todas linguagens)
# =====================
plt.figure(figsize=(10, 6))

for lang in df["Linguagem"].unique():
    subset = df[df["Linguagem"] == lang]
    plt.scatter(
        subset["CPU_norm"],
        subset["Tempo (s)"],
        label=lang,
        alpha=0.7
    )

plt.title("Tempo de Execução vs Uso de CPU Normalizado (0-100%)")
plt.xlabel("CPU Normalizado (%)")
plt.ylabel("Tempo (s)")
plt.legend(title="Linguagem")
plt.grid(True, linestyle="--", alpha=0.5)
plt.savefig("results/dispersao_todas.png", dpi=300, bbox_inches="tight")
plt.close()

# =====================
# Gráficos individuais
# =====================
for lang in ["Ambar", "C", "Rust"]:
    subset = df[df["Linguagem"] == lang]

    plt.figure(figsize=(8, 5))
    plt.scatter(
        subset["CPU_norm"],
        subset["Tempo (s)"],
        label=lang,
        color="tab:blue",
        alpha=0.7
    )

    plt.title(f"Tempo de Execução vs Uso de CPU Normalizado (0-100%) - {lang}")
    plt.xlabel("CPU Normalizado (%)")
    plt.ylabel("Tempo (s)")
    plt.grid(True, linestyle="--", alpha=0.5)
    plt.legend()
    plt.savefig(f"results/dispersao_{lang.lower()}.png", dpi=300, bbox_inches="tight")
    plt.close()

print("✅ Gráficos salvos em 'results/'")

