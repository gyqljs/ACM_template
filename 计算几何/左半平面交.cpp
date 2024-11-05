#include <bits/stdc++.h>

#define double long double

const double eps = 1e-12;

template<typename T>
using poi = std::pair<T, T>;

#define x first
#define y second

template<typename T>
poi<T> operator+(poi<T> a, poi<T> b) {
  return poi<T>(a.x + b.x, a.y + b.y);
}

template<typename T>
poi<T> operator-(poi<T> a, poi<T> b) {
  return poi<T>(a.x - b.x, a.y - b.y);
}

template<typename T>
poi<T> operator*(T k, poi<T> a) {
  return poi<T>(a.x * k, a.y * k);
}

template<typename T>
poi<T> operator*(poi<T> a, T k) {
  return poi<T>(a.x * k, a.y * k);
}

template<typename T>
T operator*(poi<T> a, poi<T> b) {
  return a.x * b.y - a.y * b.x;
}

template<typename T>
struct line {
  poi<T> u, v;
};

template<typename T>
poi<T> get_poi(line<T> a, line<T> b) {
  poi<T> p = a.u - b.u, q = a.v - a.u, w = b.v - b.u;
  T t = p * w / (w * q);
  return a.u + q * t;
} // 求两直线交点


template<typename T>
int poi_pos(line<T> a, poi<T> b) {
  if (fabs((a.v - a.u) * (b - a.u)) < eps)
    return 0;
  return (a.v - a.u) * (b - a.u) > eps ? 1 : -1;
} // 点在直线向量方向的左侧为1，右侧为-1，直线上为0


template<typename T>
double ang(line<T> &a) {
  return atan2(a.v.y - a.u.y, a.v.x - a.u.x);
}


template<typename T>
bool cmp(line<T> &a, line<T> &b) { // 按极角+左侧排序
  double A = ang(a), B = ang(b);
  return fabs(A - B) > eps ? A < B : (a.v - a.u) * (b.v - a.u) < 0;
}


// 左半平面交
template<typename T>
std::deque<line<T>> half_plane(std::vector<line<T>> &a) {
  std::deque<line<T>> q;
  q.push_back(a[0]);
  for (int i = 1; i < a.size(); i++) { // 枚举直线
    if (ang(a[i]) - ang(a[i - 1]) < eps)
      continue;
    while (q.size() > 1 && poi_pos(a[i], get_poi(q.back(), q[q.size() - 2])) == -1)
      q.pop_back();
    while (q.size() > 1 && poi_pos(a[i], get_poi(q.front(), q[1])) == -1)
      q.pop_front();
    q.push_back(a[i]);
  }
  while (q.size() > 1 and poi_pos(q[0], get_poi(q.back(), q[q.size() - 2])) == -1)
    q.pop_back();

  q.push_back(q.front()); // 封口
  return q;
}