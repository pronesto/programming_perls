#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <string>
#include <numeric> // for std::accumulate

using namespace std;

const int N = 2048; 

// Simple checksum: Sum of all elements in C
double get_checksum(const vector<float>& C) {
    double sum = 0;
    for (float val : C) {
        sum += (double)val; // Use double to prevent precision loss during summation
    }
    return sum;
}

void naive_mm(const float* A, const float* B, float* C) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            float sum = 0;
            for (int k = 0; k < N; ++k) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

void ikj_mm(const float* A, const float* B, float* C) {
    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < N; ++k) {
            float r = A[i * N + k];
            for (int j = 0; j < N; ++j) {
                C[i * N + j] += r * B[k * N + j];
            }
        }
    }
}

// Optimized iterative kernel for base case
__attribute__((always_inline)) inline void ikj_kernel(const float* A, const float* B, float* C, 
                int rA, int cA, int rB, int cB, int rC, int cC, 
                int size, int fullN) {
    for (int i = 0; i < size; ++i) {
        for (int k = 0; k < size; ++k) {
            float r = A[(rA+i) * fullN+(cA+k)];
            for (int j = 0; j < size; ++j) {
                C[(rC+i) * fullN+(cC+j)] += r * B[(rB+k) * fullN+(cB+j)];
            }
        }
    }
}

void tiled_mm_unified(const float* A, const float* B, float* C, int TS) {
    for (int i0 = 0; i0 < N; i0 += TS) {
        for (int k0 = 0; k0 < N; k0 += TS) {
            for (int j0 = 0; j0 < N; j0 += TS) {
                int size = min(TS, N - i0);
                ikj_kernel(A, B, C, i0, k0, k0, j0, i0, j0, size, N);
            }
        }
    }
}

void report(string label, double ms, double checksum) {
    cout << left << setw(20) << label 
         << setw(15) << fixed << setprecision(2) << ms << " ms" 
         << " | Checksum: " << scientific << setprecision(4) << checksum << endl;
}

int main() {
    vector<float> A(N * N, 1.0f);
    vector<float> B(N * N, 2.0f);
    vector<float> C(N * N, 0.0f);

    cout << "Matrix Multiplication Benchmarks (N=" << N << "):" << endl;
    cout << "-----------------------------------------------------------------------" << endl;

    // 1. Naive Baseline
    fill(C.begin(), C.end(), 0.0f);
    auto start = chrono::high_resolution_clock::now();
    // naive_mm(A.data(), B.data(), C.data());
    auto end = chrono::high_resolution_clock::now();
    report("Naive (ijk)", chrono::duration<double, milli>(end - start).count(), get_checksum(C));

    // 2. Optimized Baseline
    fill(C.begin(), C.end(), 0.0f);
    start = chrono::high_resolution_clock::now();
    ikj_mm(A.data(), B.data(), C.data());
    end = chrono::high_resolution_clock::now();
    report("Optimized (ikj)", chrono::duration<double, milli>(end - start).count(), get_checksum(C));

    cout << "\nTiled Version Results:" << endl;
    cout << "-----------------------------------------------------------------------" << endl;
    
    vector<int> tile_sizes = {32, 64, 128, 256, 512, 1024, 2048};
    for (int ts : tile_sizes) {
        fill(C.begin(), C.end(), 0.0f);
        start = chrono::high_resolution_clock::now();
        tiled_mm_unified(A.data(), B.data(), C.data(), ts);
        end = chrono::high_resolution_clock::now();
        report("Tile Size " + to_string(ts), chrono::duration<double, milli>(end - start).count(), get_checksum(C));
    }

    return 0;
}
