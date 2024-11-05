#include <bits/stdc++.h>

template<typename T>
using poi = std::pair<T, T>;
#define x first
#define y second

// 对于某个凸包,凸包上一定存在一点,使得凸包上的边从这一点连出的边按照顺时针或者逆时针开始极角有序

// 大于0表示c 在向量 ab 的逆时针方向
template<typename T>
T cross(poi<T> a, poi<T> b, poi<T> c) { // 叉积
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template<typename T>
std::vector<poi<T>> andrew(std::vector<poi<T>> &p) {
  std::vector<poi<T>> s;
  std::sort(p.begin(), p.end());
  int n = p.size();
  auto check = [&](int i) {
    return cross(s[s.size() - 2], s.back(), p[i]) <= 0;
  };
  // 下凸包
  for (int i = 0; i < n; i++) {
    while (s.size() > 1 and check(i)) {
      s.pop_back();
    }
    s.push_back(p[i]);
  }
  int k = s.size();
  // 上凸包
  for (int i = n - 2; i >= 0; i--) {
    while (s.size() > k and check(i)) {
      s.pop_back();
    }
    s.push_back(p[i]);
  }
  s.pop_back();
  return s;
}
