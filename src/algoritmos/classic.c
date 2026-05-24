#include "matriz/matriz.h"

/* Multiplicacion clasica: C = A * B.
 * Orden de loops i-k-j para localidad de caches (fila de A y fila de B
 * recorridas secuencialmente; fila de C acumulada). */
void mul_classic(const Matriz A, const Matriz B, Matriz C) {
    const size_t n = A.n;
    matriz_ceros(C);
    for (size_t i = 0; i < n; ++i) {
        for (size_t k = 0; k < n; ++k) {
            double aik = A.datos[i * n + k];
            for (size_t j = 0; j < n; ++j) {
                C.datos[i * n + j] += aik * B.datos[k * n + j];
            }
        }
    }
}
