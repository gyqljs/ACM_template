#include <bits/stdc++.h>

template<typename T>
using poi = std::pair<T, T>;
#define x first
#define y second


// 平面最远距离相当于凸包直径
template<typename T>
T cross(poi<T> &a, poi<T> &b, poi<T> &c) {
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template<typename T>
T rc(std::vector<poi<T>> s) { // 旋转卡壳
  int n = s.size();
  T res = 0;
  for (int i = 0, j = 1; i < n; i++) {
    while (cross(s[i], s[(i + 1) % n], s[j]) < cross(s[i], s[(i + 1) % n], s[(j + 1) % n]))
      j = (j + 1) % n;
    res = std::max(res, std::max(dis(s[i], s[j]), (T) dis(s[(i + 1) % n], s[j])));
  }
  return res;
}
