import pandas as pd
import matplotlib.pyplot as plt
import sys

# =========================
# 1️⃣ Ler CSV
# =========================
csv_file = "metrics_nbody_energy.csv"
df = pd.read_csv(csv_file)

print(f"✅ Arquivo '{csv_file}' carregado com colunas: {list(df.columns)}")

# =========================
# 2️⃣ Detectar coluna de energia automaticamente
# =========================
energia_col = None
for col in df.columns:
    if "energia" in col.lower() or "joule" in col.lower() or "energy" in col.lower():
        energia_col = col
        break

if energia_col:
    print(f"🔋 Coluna de energia detectada: '{energia_col}'")
else:
    print("⚠️ Nenhuma coluna de energia encontrada. Apenas o gráfico de tempo será gerado.")

# =========================
# 3️⃣ Normalizar tempo em relação à linguagem C
# =========================
if "Tempo_Medio_s" not in df.columns:
    sys.exit("❌ A coluna 'Tempo_Medio_s' não foi encontrada no CSV.")

c_time = df.loc[df["Linguagem"] == "C", "Tempo_Medio_s"].values[0]
df["Tempo_Relativo"] = df["Tempo_Medio_s"] / c_time

# =========================
# 4️⃣ Se houver energia, calcular EDP e normalizar
# =========================
if energia_col:
    df["EDP"] = df[energia_col] * df["Tempo_Medio_s"]
    c_edp = df.loc[df["Linguagem"] == "C", "EDP"].values[0]
    df["EDP_Relativo"] = df["EDP"] / c_edp

# =========================
# 5️⃣ Gráfico combinado: Tempo vs EDP
# =========================
plt.figure(figsize=(9, 5))
x = range(len(df))
bar_width = 0.4

# Barras de tempo (azul)
bars1 = plt.bar([i - bar_width/2 for i in x], df["Tempo_Relativo"],
                width=bar_width, label="Tempo relativo", color="#2d7bf4")

# Barras de EDP (verde), se disponível
if energia_col:
    bars2 = plt.bar([i + bar_width/2 for i in x], df["EDP_Relativo"],
                    width=bar_width, label="EDP relativo", color="#2df46e")

plt.xticks(x, df["Linguagem"])
plt.ylabel("Relativo a C (C = 1x)")
plt.title("Comparação de Desempenho e Eficiência Energética Relativa")
plt.grid(axis="y", linestyle="--", alpha=0.7)
plt.legend()

# Mostrar valores no topo das barras
def label_bars(bars):
    for bar in bars:
        h = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2.0, h + 0.02, f"{h:.2f}x",
                 ha='center', va='bottom', fontsize=9)

label_bars(bars1)
if energia_col:
    label_bars(bars2)

plt.tight_layout()
plt.savefig("metrics_relative_combined.png")
plt.show()

# =========================
# 6️⃣ Exibir resumo no terminal
# =========================
cols_to_show = ["Linguagem", "Tempo_Medio_s", "Tempo_Relativo"]
if energia_col:
    cols_to_show += [energia_col, "EDP", "EDP_Relativo"]

print("\n📊 Resumo de métricas normalizadas:\n")
print(df[cols_to_show].to_string(index=False))


