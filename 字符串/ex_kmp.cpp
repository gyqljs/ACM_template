#include <bits/stdc++.h>

auto get_z(const std::string &t) {
  std::vector<int> z(t.size());
  int l = 0, r = -1;
  z[0] = t.size();
  // t(i, i + z[i] - 1) 一定是t的前缀
  for (int i = 1; i < t.size(); i++) {
    if (i <= r) {
      z[i] = std::min(r - i + 1, z[i - l]);
    }
    while (i + z[i] < t.size() && t[z[i]] == t[i + z[i]]) {
      z[i]++;
    }
    if (i + z[i] - 1 > r) {
      l = i;
      r = i + z[i] - 1;
    }
  }
  return z;
}

auto ex_kmp(const std::string &b, const std::string &a) {
  auto z = get_z(a);
  std::vector<int> p(b.size());
  int l = 0, r = -1;
  for (int i = 0; i < b.size(); i++) {
    if (i <= r) {
      p[i] = std::min(r - i + 1, z[i - l]);
    }
    while (i + p[i] < b.size() && p[i] < a.size() && b[i + p[i]] == a[p[i]]) {
      p[i]++;
    }
    if (i + p[i] - 1 > r) {
      l = i;
      r = i + p[i] - 1;
    }
  }
}