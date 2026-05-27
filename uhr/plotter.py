import matplotlib.pyplot as plt
import pandas as pd

# Cargar los 3 algoritmos
clasico = pd.read_csv("resultados/clasico_int.csv")
hibrido = pd.read_csv("resultados/hibrido_int.csv")
strassen = pd.read_csv("resultados/strassen_int.csv")

fig, ax = plt.subplots(figsize=(8, 6))

# Lista para iterar y graficar los tres
algoritmos = [
    ("Clásico", clasico, 'o', 'tab:blue'),
    ("Híbrido", hibrido, 's', 'tab:orange'),
    ("Strassen", strassen, '^', 'tab:green')
]

for nombre, df, marcador, color in algoritmos:
    ax.errorbar(df["n"], df["t_mean"], yerr=df["t_stdev"], fmt=f'-{marcador}', color=color, label=nombre)

# Configurar el gráfico único (Log-Log)
ax.set_title("Comparación de Algoritmos en Matriz de Números Enteros (Escala Log-Log)")
ax.set_xlabel("Tamaño Matriz (N)")
ax.set_ylabel("Tiempo (us)")
ax.set_xscale("log")
ax.set_yscale("log")
ax.grid(True, which="both", ls="--", alpha=0.5)
ax.legend()

fig.tight_layout()
fig.savefig("graficos/sample_plots.png")
