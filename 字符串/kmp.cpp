#include <bits/stdc++.h>

std::vector<int> get_n(std::string &t) {
  // next数组存储了模式串t的最长前后缀匹配长度
  int n = t.size();
  std::vector<int> next(n);
  for (int i = 1, k = 0; i < n; i++) {
    while (k > 0 && t[k] != t[i]) {
      k = next[k - 1];
    }
    if (t[k] == t[i]) {
      k++;
    }
    next[i] = k;
  }
  return next;
}

// 获取字符串s的最小循环字串
std::string get_min_cyclic_substring(std::string &s) {
  std::vector<int> lps = get_n(s); // 获取部分匹配表
  int n = s.size();
  int p = n - lps[n - 1]; // 计算最小周期长度

  // 检查是否存在循环周期
  if (p != n && n % p == 0) {
    return s.substr(0, p); // 返回最小循环子串
  } else {
    return s; // 没有循环周期，返回原字符串
  }
}


void kmp(std::string &s, std::string &t) {
  auto next = get_n(t);
  int n = s.size();
  for (int i = 0, k = 0; i < t.size(); i++) {
    while (k > 0 && s[k] != t[i]) {
      k = next[k - 1];
    }
    if (s[k] == t[i]) {
      k++;
    }
    if (k == n + 1) {
      std::cout << i - n + 1 << '\n';
      k = next[k - 1];
    }
  }
  for (int i = 0; i <= n; i++) {
    std::cout << next[i] << " ";
  }
  std::cout << '\n';
}
