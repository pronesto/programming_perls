# Tiling Matrix Multiplication

Matrix multiplication *C = A times B* is the fundamental "workhorse" of modern computing. It powers everything from 3D graphics and physics simulations to the massive neural networks used in modern AI. However, as matrices grow to sizes like , the main bottleneck isn't the CPU's speed—it's the **memory wall**. Traditional nested-loop multiplication often forces the computer to fetch data from the slow main RAM repeatedly because the data doesn't fit in the fast, local CPU cache.

**Tiling** (also known as blocking) solves this by breaking the large matrices into smaller "tiles" or sub-matrices. By processing one tile at a time, the computer can keep all the necessary data within the CPU's L1 or L2 cache, drastically reducing memory latency and increasing throughput.

While traditional tiling uses fixed-size loops, a **recursive divide-and-conquer approach** achieves tiling naturally. By recursively splitting the matrices into four smaller quadrants, the algorithm eventually reaches "base case" sub-problems that fit perfectly into the cache, regardless of the specific hardware architecture. Here's the approach:

```C++
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
```
