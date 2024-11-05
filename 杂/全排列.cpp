#include <iostream>
#include <algorithm>
#include <vector>

int main() {
  // 初始化待排列的序列
  std::vector<int> nums = {1, 2, 3};

  // 先对序列进行排序，以确保全排列从字典序最小的排列开始
  std::sort(nums.begin(), nums.end());

  // 使用do-while循环生成所有排列
  do {
    // 输出当前排列
    for (int num: nums) {
      std::cout << num << " ";
    }
    std::cout << std::endl;
  } while (std::next_permutation(nums.begin(), nums.end()));

  return 0;
}
