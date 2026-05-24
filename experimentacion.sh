#!/bin/bash
mkdir -p resultados

echo "Iniciando batería de experimentos automática..."

# Modificadores:
# Argumento 6 (Tipo Matriz): 1=Enteros, 2=Reales, 3=Estructurada, 4=Rala
# Argumento 7 (Algoritmo): 1=Clásico, 2=Strassen
# EXPERIMENTOS ALGORITMO CLÁSICO

echo "Corriendo Clásico - Enteros..."
./benchmark.exe resultados/clasico_enteros.csv 32 2 512 2 1 1

echo "Corriendo Clásico - Reales..."
./benchmark.exe resultados/clasico_reales.csv 32 2 512 2 2 1

echo "Corriendo Clásico - Estructuradas..."
./benchmark.exe resultados/clasico_estructuradas.csv 32 2 512 2 3 1

echo "Corriendo Clásico - Ralas..."
./benchmark.exe resultados/clasico_ralas.csv 32 2 512 2 4 1


# EXPERIMENTOS ALGORITMO STRASSEN
echo "Corriendo Strassen - Enteros..."
./benchmark.exe resultados/strassen_enteros.csv 32 2 512 2 1 2

echo "Corriendo Strassen - Reales..."
./benchmark.exe resultados/strassen_reales.csv 32 2 512 2 2 2

echo "Corriendo Strassen - Estructuradas..."
./benchmark.exe resultados/strassen_estructuradas.csv 32 2 512 2 3 2

echo "Corriendo Strassen - Ralas..."
./benchmark.exe resultados/strassen_ralas.csv 32 2 512 2 4 2

echo " Todos los datos generados"