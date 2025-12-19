#include <iostream>
#include <vector>
#include <algorithm> // for std::max

int max_subarray_sum(const std::vector<int>& nums) {
  if (nums.empty()) return 0;

  int current_max = nums[0];
  int global_max = nums[0];

  for (size_t i = 1; i < nums.size(); ++i) {
    current_max = std::max(nums[i], current_max + nums[i]);
    global_max = std::max(global_max, current_max);
  }

  return global_max;
}

int main() {
  std::vector<int> numbers = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
  std::cout << "Max Sum: " << max_subarray_sum(numbers) << std::endl;
  return 0;
}
