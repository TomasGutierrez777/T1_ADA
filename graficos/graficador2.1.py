import matplotlib.pyplot as plt
import pandas as pd
import os

os.makedirs("resultados", exist_ok=True)

experimentos = ["enteros", "reales", "estructuradas", "ralas"]

for exp in experimentos:
    ruta_csv = f"resultados/{exp}.csv"

    # Validar si el archivo existe antes de graficar
    if not os.path.exists(ruta_csv):
        print(f"No se encontró {ruta_csv}, saltando...")
        continue

    print(f"Generando gráfico para: {exp}...")
    df = pd.read_csv(ruta_csv)

    # Crear un gráfico único y limpio para cada uno
    fig, ax = plt.subplots(figsize=(7, 5))

    # Dibujar los puntos reales con sus barras de error (desviación estándar)
    ax.errorbar(
        df["n"],
        df["t_mean"],
        yerr=df["t_stdev"],  #
        linestyle="None",
        marker='.',
        ecolor='tab:red',
        label="Datos Reales (Media)"
    )

    # Configurar títulos y etiquetas formales
    ax.set_title(f"Análisis de Rendimiento - Matrices {exp.capitalize()}", fontsize=12, fontweight='bold')
    ax.set_xlabel("Tamaño de la matriz (N)", fontsize=10)
    ax.set_ylabel("Tiempo de ejecución (Microsegundos)", fontsize=10)

    # ESCALA LOG-LOG ASINTÓTICA
    ax.set_xscale("log")
    ax.set_yscale("log")

    ax.grid(True, which="both", linestyle="--", alpha=0.5)
    ax.legend()

    # Guardar la imagen directo en la carpeta de resultados
    fig.tight_layout()
    nombre_imagen = f"resultados/grafico_{exp}.png"
    fig.savefig(nombre_imagen, dpi=300) # dpi=300 para que se vea nítido en Word
    plt.close(fig)

print("¡Todos los gráficos generados en /resultados, wn")