# Benchmark: Multiplicación de Matrices — Clásico vs Strassen

Tarea 1 — Análisis de Algoritmos (501404 / 503309)

Implementación y benchmarking experimental de los algoritmos de multiplicación de matrices **Clásico** y **Strassen** (incluyendo variante híbrida), usando la suite **uhr** para reportar tiempos promedio, desviación estándar y cuartiles sobre al menos 32 ejecuciones por tamaño.

---

## Algoritmos implementados

| ID | Algoritmo | Complejidad |
|----|-----------|-------------|
| `1` | Clásico | $\Theta(n^3)$ |
| `2` | Strassen puro | $\Theta(n^{\log_2 7}) \approx \Theta(n^{2.807})$ |
| `3` | Strassen híbrido | Strassen hasta $n \geq n_0$, luego Clásico |

---

## Requisitos

- `g++` con soporte C++17 o superior
- `python3` con `matplotlib`, `numpy` (para graficar)

---

## Compilación

```bash
g++ -std=c++17 uhr/uhr.cpp uhr/utils.cpp uhr/quartiles_nth.cpp \
    -x c++ src/matriz/matriz.c src/algoritmos/classic.c src/algoritmos/strassen.c \
    -I uhr -I src -I src/matriz -I src/algoritmos \
    -o benchmark -lm
```

---

## Uso

```
./benchmark <salida.csv> <n_min> <paso> <n_max> <paso> <runs> <tipo_matriz> <algoritmo> [n0]
```

### Parámetros

| Parámetro      | Descripción |
|----------------|-------------|
| `salida.csv`   | Archivo de salida con los resultados |
| `n_min`        | Tamaño mínimo de matriz (potencia de 2) |
| `n_max`        | Tamaño máximo de matriz (potencia de 2) |
| `paso`         | Factor de crecimiento entre tamaños |
| `runs`         | Número de ejecuciones por tamaño (mínimo 32) |
| `tipo_matriz`  | Tipo de instancia (ver tabla abajo) |
| `algoritmo`    | Algoritmo a ejecutar (1, 2 o 3) |
| `n0`           | *(Solo algoritmo 3)* Umbral de corte para el híbrido |

### Tipos de matriz

| ID | Descripción |
|----|-------------|
| `1` | Enteros aleatorios en $[-100, 100]$ |
| `2` | Reales aleatorios en $[0, 1]$ |
| `3` | Matriz identidad (estructurada) |
| `4` | Matriz rala (90% ceros) |

### Ejemplos

```bash
# Strassen puro, enteros aleatorios, n de 32 a 512
./benchmark resultados/strassen_int.csv 32 2 512 2 32 1 2

# Clásico, enteros aleatorios
./benchmark resultados/clasico_int.csv 32 2 512 2 32 1 1

# Strassen puro, reales aleatorios
./benchmark resultados/strassen_real.csv 32 2 512 2 32 2 2

# Strassen híbrido con umbral n0=64, matrices ralas
./benchmark resultados/hibrido_rala.csv 32 2 512 2 32 4 3 64
```

Los resultados se guardan como CSV con columnas: `n, t_mean, t_stdev, t_Q0, t_Q1, t_Q2, t_Q3, t_Q4` (tiempos en nanosegundos).

