# Kadane's Algorithm: A Functional Perspective

This repository implements **Kadane's Algorithm** (Maximum Subarray Sum) using two distinct paradigms: **Modern C++23** (using Ranges and Views) and **Standard ML (SML/NJ)**.

The goal is to demonstrate how modern C++ syntax has evolved to mirror the elegance of purely functional languages.

## The Logic

Kadane’s algorithm is a dynamic programming approach that finds the contiguous subarray with the largest sum in  time. It works by making a local choice at each element: "Is it better to start a new subarray here, or continue the previous one?"

---

## Modern C++23 Implementation

Traditionally, C++ implementations of Kadane’s involved `for` loops with manual index management. The code below leverages **C++23 Ranges**, making the code feel like a data pipeline.

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>

int max_subarray_sum(const std::vector<int>& nums) {
    if (nums.empty()) return 0;

    int global_max = nums[0];
    int current_max = nums[0];

    // Functional Pipe: Skip the first element and iterate over the rest
    auto remaining_elements = nums | std::views::drop(1);

    for (int x : remaining_elements) {
        current_max = std::max(x, current_max + x);
        global_max = std::max(global_max, current_max);
    }

    return global_max;
}

```

### Why it feels functional:

* **`std::views::drop(1)`**: Instead of `i = 1`, we create a "view" of the data, treating the collection as a stream.
* **Declarative Nature**: The focus is on *what* the data is (the remaining elements) rather than *how* to increment the pointer.

---

## SML/NJ Implementation (The Functional Root)

To see where these ideas come from, we look at the SML implementation. It uses **Pattern Matching** and **Tail Recursion** instead of mutable state loops.

```sml
fun max(x, y) = if x > y then x else y

(* Recursive helper using pattern matching *)
fun kadane nil _ glob = glob
  | kadane (h::t) curr glob =
    let
      val next_curr = max(h, curr + h)
      val next_glob = max(glob, next_curr)
    in
      kadane t next_curr next_glob
    end

fun find_max_subarray (h::t) = kadane t h h

```

## Requirements

* **C++:** A compiler supporting C++23 (e.g., GCC 12+ or Clang 15+).
* **SML:** Standard ML of New Jersey (SML/NJ) interpreter.
