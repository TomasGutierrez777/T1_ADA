/** uhr: generic time performance tester
 * Author: LELE
 *
 * Things to set up:
 * 0. Includes: include all files to be tested,
 * 1. Time unit: in elapsed_time,
 * 2. What to write on time_data,
 * 3. Data type and distribution of RNG,
 * 4. Additive or multiplicative stepping,
 * 5. The experiments: in outer for loop. */

#include <cstdint>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <string>

#include "utils.cpp"

// --- INCLUSIÓN DE TU CABECERA DIRECTA EN C++ ---
#include "matriz.h"

// --- VARIABLES GLOBALES DEL EXPERIMENTO ---
static int TIPO_MATRIZ = 1;
static int ALGORITMO = 1;   // 1: Clásico, 2: Strassen Puro, 3: Strassen Híbrido
static int CORTE_N0 = 2;

// --- FUNCIONES DE CONFIGURACIÓN Y LLENADO ---
inline void capturar_configuracion(int argc, char *argv[]) {
    std::cout << "[DEBUG] Total argc: " << argc << std::endl;
    for(int i = 0; i < argc; i++) {
        std::cout << "  argv[" << i << "]: " << argv[i] << "\n";
    }

    if (argc >= 7) {
        TIPO_MATRIZ = std::stoi(argv[6]);
    }
    if (argc >= 8) {
        ALGORITMO = std::stoi(argv[7]);
    }
    if (argc >= 9) {
        CORTE_N0 = std::stoi(argv[8]);
    }

    std::cout << "[DEBUG] Config Final -> TIPO_MATRIZ: " << TIPO_MATRIZ
              << " | ALGORITMO: " << ALGORITMO << std::endl;
}

inline void rellenar_matriz_experimento(Matriz M) {
    std::random_device rd;
    std::mt19937_64 rng(rd());

    int tam = M.n;

    if (TIPO_MATRIZ == 1) { // 1. ENTEROS
        std::uniform_int_distribution<int> dist(-100, 100);
        for(int i = 0; i < tam * tam; i++) M.datos[i] = dist(rng);
    }
    else if (TIPO_MATRIZ == 2) { // 2. REALES (0 a 1)
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        for(int i = 0; i < tam * tam; i++) M.datos[i] = dist(rng);
    }
    else if (TIPO_MATRIZ == 3) { // 3. ESTRUCTURADAS (Identidad)
        for(int r = 0; r < tam; r++) {
            for(int c = 0; c < tam; c++) {
                M.datos[r * tam + c] = (r == c) ? 1.0 : 0.0;
            }
        }
    }
    else if (TIPO_MATRIZ == 4) { // 4. RALAS (90% ceros)
        std::uniform_int_distribution<int> dist(-100, 100);
        std::uniform_int_distribution<int> prob(1, 10);
        for(int i = 0; i < tam * tam; i++) M.datos[i] = (prob(rng) == 1) ? dist(rng) : 0.0;
    }
}

// --- MAIN DEL BENCHMARK ---
int main(int argc, char *argv[])
{
    // Capturamos las banderas de la consola ANTES de que `validate_input` limpie parámetros
    capturar_configuracion(argc, argv);

    // Validate and sanitize input
    std::int64_t runs, lower, upper, step;
    validate_input(argc, argv, runs, lower, upper, step);

    if (step < 2) {
        std::cerr << "<STEP> debe ser >= 2 (stepping multiplicativo)." << std::endl;
        return EXIT_FAILURE;
    }

    // Set up clock variables
    std::int64_t n, i, executed_runs;
    std::int64_t total_runs_multiplicative = runs * (std::int64_t(floor(log(upper / double(lower)) / log(step))) + 1);
    std::vector<double> times(runs);
    std::vector<double> q;
    double mean_time, time_stdev, dev;
    auto begin_time = std::chrono::high_resolution_clock::now();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> elapsed_time = end_time - begin_time;

    // Set up random number generation
    std::random_device rd;
    std::mt19937_64 rng(rd());
    std::uniform_int_distribution<std::int64_t> u_distr; // change depending on app

    // File to write time data
    std::ofstream time_data;
    time_data.open(argv[1]);
    time_data << "n,t_mean,t_stdev,t_Q0,t_Q1,t_Q2,t_Q3,t_Q4" << std::endl;

    // Begin testing
    std::cerr << "\033[0;36mRunning tests...\033[0m" << std::endl << std::endl;
    executed_runs = 0;
    for (n = lower; n <= upper; n *= step) {
        mean_time = 0;
        time_stdev = 0;

        // Run to compute elapsed time
        for (i = 0; i < runs; i++) {
            display_progress(++executed_runs, total_runs_multiplicative);

            // 1. PREPARACIÓN (Fuera del reloj)
            Matriz A = crear_matriz(n);
            Matriz B = crear_matriz(n);
            Matriz C = crear_matriz(n);

            rellenar_matriz_experimento(A);
            rellenar_matriz_experimento(B);
            matriz_ceros(C);

            // 2. MEDICIÓN CRÍTICA DE TIEMPO
            begin_time = std::chrono::high_resolution_clock::now();

            if (ALGORITMO == 1) {
                mul_classic(A, B, C);
            }
            else if (ALGORITMO == 2) {
                mul_strassen(A, B, C);
            }
            else if (ALGORITMO == 3) {
                mul_hybrid(A, B, C, static_cast<size_t>(CORTE_N0));
            }

            end_time = std::chrono::high_resolution_clock::now();

            // 3. LIMPIEZA DE MEMORIA (Fuera del reloj)
            liberar_matriz(A);
            liberar_matriz(B);
            liberar_matriz(C);

            elapsed_time = end_time - begin_time;
            times[i] = elapsed_time.count();

            mean_time += times[i];
        }

        // Compute statistics
        mean_time /= runs;

        for (i = 0; i < runs; i++) {
            dev = times[i] - mean_time;
            time_stdev += dev * dev;
        }

        time_stdev /= runs - 1; // Subtract 1 to get unbiased estimator
        time_stdev = std::sqrt(time_stdev);

        quartiles(times, q);

        time_data << n << "," << mean_time << "," << time_stdev << ",";
        time_data << q[0] << "," << q[1] << "," << q[2] << "," << q[3] << "," << q[4] << std::endl;
    }

    // This is to keep loading bar after testing
    std::cerr << std::endl << std::endl;
    std::cerr << "\033[1;32mDone!\033[0m" << std::endl;

    time_data.close();

    return 0;
}