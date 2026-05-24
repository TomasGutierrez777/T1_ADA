#ifndef MATRIZ_H
#define MATRIZ_H

#include <stddef.h>

// Definición limpia en español
typedef struct {
    size_t n;        // Dimensión de la matriz (n x n)
    double *datos;   // El bloque de memoria donde se guardan los números
} Matriz;

// Funciones básicas para gestionar la memoria de las matrices
Matriz crear_matriz(size_t n);
void liberar_matriz(Matriz m);
void matriz_ceros(Matriz m);

// Tus algoritmos de multiplicación
void mul_classic(const Matriz A, const Matriz B, Matriz C);
void mul_strassen(const Matriz A, const Matriz B, Matriz C);
void mul_hybrid(const Matriz A, const Matriz B, Matriz C, size_t n0);

#endif