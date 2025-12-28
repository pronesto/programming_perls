#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <algorithm>

using namespace std;

const int N = 2048; 

// Helper to verify results
double get_checksum(const vector<float>& C) {
  double sum = 0;
  for (float val : C) {
    sum += (double)val;
  }
  return sum;
}

// Optimized iterative kernel for base case
__attribute__((always_inline)) inline
void ikj_kernel(const float* A, const float* B, float* C, 
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

// Recursive function
void dc_mm(const float* A, const float* B, float* C, 
    int rA, int cA, int rB, int cB, int rC, int cC, 
    int size, int TS) {
  if (size <= TS) {
    ikj_kernel(A, B, C, rA, cA, rB, cB, rC, cC, size, N);
    return;
  }
  int NN = size / 2;
  dc_mm(A, B, C, rA, cA, rB, cB, rC, cC, NN, TS);
  dc_mm(A, B, C, rA, cA+NN, rB+NN, cB, rC, cC, NN, TS);
  dc_mm(A, B, C, rA, cA, rB, cB+NN, rC, cC+NN, NN, TS);
  dc_mm(A, B, C, rA, cA+NN, rB+NN, cB+NN, rC, cC+NN, NN, TS);
  dc_mm(A, B, C, rA+NN, cA, rB, cB, rC+NN, cC, NN, TS);
  dc_mm(A, B, C, rA+NN, cA+NN, rB+NN, cB, rC+NN, cC, NN, TS);
  dc_mm(A, B, C, rA+NN, cA, rB, cB+NN, rC+NN, cC+NN, NN, TS);
  dc_mm(A, B, C, rA+NN, cA+NN, rB+NN, cB+NN, rC+NN, cC+NN, NN, TS);
}

int main() {
  vector<float> A(N * N, 1.0f);
  vector<float> B(N * N, 2.0f);
  vector<float> C(N * N, 0.0f);

  cout << "D&C Matrix Multiplication (N=" << N << "):" << endl;
  cout << "----------------------------------------------------------" << endl;
  cout << left << setw(15);
  cout << "Threshold" << setw(15);
  cout << "Time (ms)" << "Checksum" << endl;
  cout << "----------------------------------------------------------" << endl;

  vector<int> TSs = {32, 64, 128, 256, 512, 1024, 2048};

  for (int t : TSs) {
    fill(C.begin(), C.end(), 0.0f); 
    auto start = chrono::high_resolution_clock::now();

    dc_mm(A.data(), B.data(), C.data(), 0, 0, 0, 0, 0, 0, N, t);

    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();

    cout << left << setw(15) << t 
      << setw(15) << fixed << setprecision(2) << ms 
      << scientific << setprecision(4) << get_checksum(C) << endl;
  }

  return 0;
}
