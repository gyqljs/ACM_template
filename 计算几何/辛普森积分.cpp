#include <bits/stdc++.h>

template<class T>
struct simp {
  T f;

  double sim(double l, double r) {
    return (f(l) + 4 * f((l + r) / 2) + f(r)) * (r - l) / 6;
  }

  simp(T func): f(func) {}

  double col(double l, double r, double eps, double st) {
    double m = (l + r) / 2;
    double sl = sim(l, m);
    double sr = sim(m, r);
    if (std::abs(sl + sr - st) <= 15 * eps)
      return sl + sr + (sl + sr - st) / 15;
    return col(l, m, eps / 2, sl) + col(m, r, eps / 2, sr);
  }

  double col(double l, double r) {
    return col(l, r, 1e-9, sim(l, r));
  }
};