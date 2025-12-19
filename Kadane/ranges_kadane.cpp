#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>

int max_subarray_sum_modern(const std::vector<int>& nums) {
    if (nums.empty()) return 0;

    int global_max = nums[0];
    int current_max = nums[0];

    // Using ranges to skip the first element and fold the rest
    auto remaining_elements = nums | std::views::drop(1);

    for (int x : remaining_elements) {
        current_max = std::max(x, current_max + x);
        global_max = std::max(global_max, current_max);
    }

    return global_max;
}

int main() {
  std::vector<int> numbers = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
  std::cout << "Max Sum: " << max_subarray_sum(numbers) << std::endl;
  return 0;
}
