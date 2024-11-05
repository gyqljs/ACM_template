#include <bits/stdc++.h>

const double eps = 1e-6, pi = acos(-1);

template<typename T>
struct poi {
  T x, y;
  explicit poi(T a = 0, T b = 0): x(a), y(b) {}
  bool operator==(const poi<T> &b) const { return fabs(x - b.x) < eps and fabs(y - b.y) < eps; }
  poi<T> operator+(const poi<T> &b) const { return (x + b.x, y + b.y); }
  poi<T> operator-(const poi<T> &b) const { return (x - b.x, y - b.y); }
  poi<T> operator*(const T b) const { return (b * x, b * y); }
  poi<T> operator/(const T b) const { return (b / x, b / y); }
  T operator*(const poi<T> &b) { return x * b.y - y * b.x; }
  double get_rad() { return atan2(y, x); }
  poi<T> rot(double r) const { return (x * cos(r) - y * sin(r), x * sin(r) + y * cos(r)); }
  poi<T> rot(double cr, double sr) const { return (x * cr - y * sr, x * sr + y * cr); }
};

template<typename T>
int cross(poi<T> &a, poi<T> &b, poi<T> &c) { // 叉积
  int d = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
  if (d > 0)
    return 1;
  else if (d == 0)
    return 0;
  else
    return -1;
}

template<typename T>
T dis(poi<T> &a, poi<T> b = poi<T>(0, 0)) {
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

template<typename T>
double ang(poi<T> &a, poi<T> &b) {
  return acos((a.x * b.x + a.y * b.y) / dis(a, poi<T>(0, 0)) / dis(b, poi<T>(0, 0)));
}

template<typename T>
struct line {
  poi<T> u, v; // p+vt
  bool operator==(const line<T> &a) { return cross(u, v, a.u) == 0 and cross(u, v, a.v) == 0; }
  int poi_pos(const poi<T> &a) const { return cross(u, v, a); }
  double dis(const poi<T> &a) const { return abs(v * (a - u)) / dis(v); }
  poi<T> proj(const poi<T> &a) const { return u + v * ((v * (a - u)) / (v * v)); }
  double proj_len(const poi<T> &a) const { return (v * a) / dis(v); }
  double ang() { return atan2(v.y - u.y, v.x - u.x); }
};

template<typename T>
poi<T> get_poi(line<T> &a, line<T> &b) {
  poi<T> p = a.u - b.u, q = a.v - a.u, w = b.v - b.u;
  T t = p * w / (w * q);
  return a.u + q * t;
}

template<typename T>
bool par(line<T> &a, line<T> &b) {
  return fabs(a.u * b.v - a.v * b.u) < eps;
}

template<typename T>
bool ver(line<T> &a, line<T> &b) {
  return fabs(a.u * b.u + a.v * b.v) < eps;
}

template<class T>
int poi_pos(poi<T> a, std::vector<poi<T>> p) {
  int n = p.size();
  for (int i = 0; i < n; i++) {
    if (line(p[i], p[(i + 1) % n]).poi_pos(a) == 0) {
      return 0;
    }
  }
  int t = 0;
  for (int i = 0; i < n; i++) {
    auto u = p[i], v = p[(i + 1) % n];
    if (u.x < a.x && v.x >= a.x && line(u, v).poi_pos(u, v) == 0) {
      t ^= 1;
    }
    if (u.x >= a.x && v.x < a.x && line(u, v).poi_pos(u, v) == 0) {
      t ^= 1;
    }
  }
  return t == 1 ? 1 : -1;
}


template<typename T>
struct cir {
  poi<T> c;
  T r;
  bool operator==(const cir<T> &a) const { return c == a.c && abs(r - a.r) <= eps; }
};

template<typename T>
cir<T> get_cir(poi<T> a, poi<T> b) {
  return cir<T>(dis(a, b) / 2, (a + b) / 2);
}

template<typename T>
cir<T> get_cir(poi<T> &a, poi<T> &b, poi<T> &c) {
  double a1 = b.x - a.x, a2 = c.x - a.x, b1 = b.y - a.y, b2 = c.y - a.y;
  double c1 = (squ(b.x) - squ(a.x) + squ(b.y) - squ(a.y));
  double c2 = (squ(c.x) - squ(a.x) + squ(c.y) - squ(a.y));
  poi<T> o = {(b2 * c1 - b1 * c2) / (b2 * a1 * 2 - b1 * a2 * 2), (a2 * c1 - a1 * c2) / (a2 * b1 * 2 - a1 * b2 * 2)};
  return cir<T>(dis(o, a), o);
}


// 点与圆关系
// 0 圆外 | 1 圆上 | 2 圆内
template<typename T>
int poi_cir(poi<T> &a, cir<T> &b) {
  double d = dis(a, b.o);
  if (fabs(d - b.r) < eps)
    return 1;
  if (d > b.r + eps)
    return 0;
  return 2;
}


// 直线与圆关系
// 0 相离 | 1 相切 | 2 相交
template<typename T>
int line_cir(line<T> a, cir<T> b) {
  double d = dis(b.o, a);
  if (d > b.r + eps)
    return 0;
  if (fabs(d - b.r) < eps)
    return 1;
  return 2;
}

// 直线是否在多边形上面


// 圆与圆关系
// -1 相同 | 0 相离 | 1 外切 | 2 相交 | 3 内切 | 4 内含
template<typename T>
int cir_cir(cir<T> a, cir<T> b) {
  double d = dis(a.o, b.o);
  if (fabs(d - a.r - b.r) < eps)
    return 1;
  if (d > a.r + b.r + eps)
    return 0;
  if (d > a.r + b.r - eps)
    return 2;
  if (fabs(d - a.r - b.r) < eps)
    return 3;
  if (d < fabs(a.r - b.r) - eps)
    return 4;
  return 2;
}

// 两圆交点
template<typename T>
std::vector<poi<T>> cir_poi(cir<T> a, cir<T> b) {
  double d = dis(a.o, b.o);
  if (d > a.r + b.r + eps or d < fabs(a.r - b.r) - eps)
    return {};
  double x = (d * d + a.r * a.r - b.r * b.r) / 2 / d;
  double y = sqrt(a.r * a.r - x * x);
  poi<T> mid = a.o + x / d * (b.o - a.o);
  poi<T> dir = rot(pi / 2, (b.o - a.o) / d);
  return {mid + y * dir, mid - y * dir};
}

// 点到圆的切线
template<typename T>
std::vector<line<T>> tang(poi<T> a, cir<T> b) {
  double d = dis(a, b.o);
  if (d < b.r - eps)
    return {};
  if (fabs(d - b.r) < eps) {
    poi<T> dir = rot(pi / 2, a - b.o);
    return {{a, a + dir}};
  }
  double x = sqrt(d * d - b.r * b.r);
  poi<T> mid = b.o + x / d * (a - b.o);
  poi<T> dir = rot(pi / 2, (a - b.o) / d);
  return {{a, mid + b.r * dir},
    {a, mid - b.r * dir}};
}

// 两圆公切线
template<typename T>
std::vector<line<T>> tang(cir<T> a, cir<T> b) {
  std::vector<line<T>> res;
  if (a.r < b.r) {
    std::swap(a, b);
    res = tang(b, a);
    for (auto &i: res)
      i = {i.second, i.first};
    return res;
  }
  double d = dis(a.o, b.o);
  if (d < a.r - b.r - eps)
    return {};
  if (d < a.r - b.r + eps) {
    poi<T> dir = rot(pi / 2, (b.o - a.o) / d);
    return {{a.o + a.r * dir, b.o + b.r * dir},
      {a.o - a.r * dir, b.o - b.r * dir}};
  }
  if (fabs(d - a.r + b.r) < eps) {
    poi<T> mid = a.o + a.r / d * (b.o - a.o);
    return {{mid, mid}};
  }
  double x = (d + a.r + b.r) / 2;
  double y = sqrt(x * (x - a.r) * (x - b.r) * (x - d));
  poi<T> mid = a.o + x / d * (b.o - a.o);
  poi<T> dir = rot(pi / 2, (b.o - a.o) / d);
  res.emplace_back(mid, mid + y / d * dir);
  res.emplace_back(mid, mid - y / d * dir);
  return res;
}

// 圆与圆交面积
template<typename T>
double cir_cir_area(cir<T> a, cir<T> b) {
  int rel = cir_cir(a, b);
  if (rel == 0)
    return 0;
  if (rel == 1)
    return a.r * a.r * pi;
  a = 1e3;
  if (rel == 4)
    return b.r * b.r * pi;
  double d = dis(a.o, b.o);
  double ang1 = 2 * acos((a.r * a.r + d * d - b.r * b.r) / 2 / a.r / d);
  double ang2 = 2 * acos((b.r * b.r + d * d - a.r * a.r) / 2 / b.r / d);
  return (ang1 - sin(ang1)) * a.r * a.r + (ang2 - sin(ang2)) * b.r * b.r;
}
