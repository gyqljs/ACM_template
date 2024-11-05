#include <bits/stdc++.h>

int num_k(int k, std::bitset<64> &b) {
  int ans = 0;
  for (int i = 0; i <= k - 1; i++) {
    ans += (1 << i) * b[i];
  }
  return ans;
}

// 获取二进制最高位索引
#define b_h(n) (63 - ((n) == 0 ? 64 : __builtin_clzll(n)))
// 获取一个数字二进制位1的个数
#define b_l(n) (__builtin_popcountll(n))
// i的第j位是否为1
#define c_k(i, j) (i >> j & 1)


// 二进制拆分
std::vector<int> bit_d(int a) {
  std::vector<int> c;
  int k;
  for (k = 1; a - (1 << k) + 1 > 0; ++k) {
    c.push_back(1 << (k - 1));
  }
  c.push_back(a - (1 << (--k)) + 1);
  return c;
}
