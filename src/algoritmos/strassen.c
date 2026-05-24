#include <stdlib.h>
#include <string.h>
#include "matriz.h"

// Copia un cuadrante  de una matriz grande M en una submatriz chica S
static void block_copy(const Matriz M, Matriz S, size_t ro, size_t co) {
    size_t h = S.n;
    size_t n = M.n;
    for (size_t i = 0; i < h; ++i) {
        memcpy(&S.datos[i * h], &M.datos[(ro + i) * n + co], h * sizeof(double));
    }
}

// Escribe una submatriz chica S dentro del cuadrante de la matriz grande M
static void block_write(Matriz M, const Matriz S, size_t ro, size_t co) {
    size_t h = S.n;
    size_t n = M.n;
    for (size_t i = 0; i < h; ++i) {
        memcpy(&M.datos[(ro + i) * n + co], &S.datos[i * h], h * sizeof(double));
    }
}

// Suma de matrices C = A + B
static void mat_add(const Matriz A, const Matriz B, Matriz C) {
    size_t total = A.n * A.n;
    for (size_t i = 0; i < total; ++i) {
        C.datos[i] = A.datos[i] + B.datos[i];
    }
}

// Resta de matrices C = A - B
static void mat_sub(const Matriz A, const Matriz B, Matriz C) {
    size_t total = A.n * A.n;
    for (size_t i = 0; i < total; ++i) {
        C.datos[i] = A.datos[i] - B.datos[i];
    }
}

// Declaración de la función recursiva central
static void strassen_rec(const Matriz A, const Matriz B, Matriz C, size_t n0);

// Paso de división, cálculo de M1..M7 y combinación de cuadrantes
static void strassen_step(const Matriz A, const Matriz B, Matriz C, size_t n0) {
    size_t n = A.n;
    size_t h = n / 2;

    // 1. Alojar memoria para los 4 cuadrantes de A y B
    Matriz A11 = crear_matriz(h), A12 = crear_matriz(h);
    Matriz A21 = crear_matriz(h), A22 = crear_matriz(h);
    Matriz B11 = crear_matriz(h), B12 = crear_matriz(h);
    Matriz B21 = crear_matriz(h), B22 = crear_matriz(h);

    // Copiar los datos reales de la matriz original a los cuadrantes
    block_copy(A, A11, 0, 0); block_copy(A, A12, 0, h);
    block_copy(A, A21, h, 0); block_copy(A, A22, h, h);
    block_copy(B, B11, 0, 0); block_copy(B, B12, 0, h);
    block_copy(B, B21, h, 0); block_copy(B, B22, h, h);

    // Alojar matrices temporales las 7 combinaciones de Strassen
    Matriz T1 = crear_matriz(h), T2 = crear_matriz(h);
    Matriz M1 = crear_matriz(h), M2 = crear_matriz(h), M3 = crear_matriz(h);
    Matriz M4 = crear_matriz(h), M5 = crear_matriz(h), M6 = crear_matriz(h), M7 = crear_matriz(h);

    /* M1 = (A11 + A22) * (B11 + B22) */
    mat_add(A11, A22, T1); mat_add(B11, B22, T2); strassen_rec(T1, T2, M1, n0);

    /* M2 = (A21 + A22) * B11 */
    mat_add(A21, A22, T1); strassen_rec(T1, B11, M2, n0);

    /* M3 = A11 * (B12 - B22) */
    mat_sub(B12, B22, T2); strassen_rec(A11, T2, M3, n0);

    /* M4 = A22 * (B21 - B11) */
    mat_sub(B21, B11, T2); strassen_rec(A22, T2, M4, n0);

    /* M5 = (A11 + A12) * B22 */
    mat_add(A11, A12, T1); strassen_rec(T1, B22, M5, n0);

    /* M6 = (A21 - A11) * (B11 + B12) */
    mat_sub(A21, A11, T1); mat_add(B11, B12, T2); strassen_rec(T1, T2, M6, n0);

    /* M7 = (A12 - A22) * (B21 + B22) */
    mat_sub(A12, A22, T1); mat_add(B21, B22, T2); strassen_rec(T1, T2, M7, n0);

    // Alojar cuadrantes para armar la matriz C de destino
    Matriz C11 = crear_matriz(h), C12 = crear_matriz(h);
    Matriz C21 = crear_matriz(h), C22 = crear_matriz(h);

    size_t hh = h * h;
    /* C11 = M1 + M4 - M5 + M7 */
    for (size_t i = 0; i < hh; ++i) C11.datos[i] = M1.datos[i] + M4.datos[i] - M5.datos[i] + M7.datos[i];
    /* C12 = M3 + M5 */
    for (size_t i = 0; i < hh; ++i) C12.datos[i] = M3.datos[i] + M5.datos[i];
    /* C21 = M2 + M4 */
    for (size_t i = 0; i < hh; ++i) C21.datos[i] = M2.datos[i] + M4.datos[i];
    /* C22 = M1 - M2 + M3 + M6 */
    for (size_t i = 0; i < hh; ++i) C22.datos[i] = M1.datos[i] - M2.datos[i] + M3.datos[i] + M6.datos[i];

    // Combinar los cuadrantes calculados en la gran matriz C final
    block_write(C, C11, 0, 0); block_write(C, C12, 0, h);
    block_write(C, C21, h, 0); block_write(C, C22, h, h);

    // Liberar TODA la memoria dinámica local para evitar memory leaks
    liberar_matriz(A11); liberar_matriz(A12); liberar_matriz(A21); liberar_matriz(A22);
    liberar_matriz(B11); liberar_matriz(B12); liberar_matriz(B21); liberar_matriz(B22);
    liberar_matriz(T1);  liberar_matriz(T2);
    liberar_matriz(M1);  liberar_matriz(M2);  liberar_matriz(M3);  liberar_matriz(M4);
    liberar_matriz(M5);  liberar_matriz(M6);  liberar_matriz(M7);
    liberar_matriz(C11); liberar_matriz(C12); liberar_matriz(C21); liberar_matriz(C22);
}

// Lógica de control de la recursión
static void strassen_rec(const Matriz A, const Matriz B, Matriz C, size_t n0) {
    size_t n = A.n;

    // Caso base absoluto del algoritmo
    if (n == 1) {
        C.datos[0] = A.datos[0] * B.datos[0];
        return;
    }

    // Si el tamaño actual cae por debajo del umbral n0, delegamos al clásico
    if (n < n0) {
        mul_classic(A, B, C);
        return;
    }

    // Si sigue siendo grande, ejecuta un paso más de Strassen
    strassen_step(A, B, C, n0);
}

// Strassen Puro
void mul_strassen(const Matriz A, const Matriz B, Matriz C) {
    strassen_rec(A, B, C, 2);
}

// Strassen Híbrido
void mul_hybrid(const Matriz A, const Matriz B, Matriz C, size_t n0) {
    strassen_rec(A, B, C, n0 < 2 ? 2 : n0);
}