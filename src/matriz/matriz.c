#include <stdlib.h>
#include "matriz.h"

Matriz crear_matriz(size_t n) {
    Matriz m;
    m.n = n;
    m.datos = (double *)malloc(n * n * sizeof(double));
    return m;
}

void liberar_matriz(Matriz m) {
    if (m.datos != NULL) {
        free(m.datos);
    }
}

void matriz_ceros(Matriz m) {
    size_t total = m.n * m.n;
    for (size_t i = 0; i < total; ++i) {
        m.datos[i] = 0.0;
    }
}