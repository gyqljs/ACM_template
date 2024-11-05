// https://www.luogu.com.cn/problem/P1883
#include <bits/stdc++.h>

const int N = 1e4 + 4;
int n, a[N], b[N], c[N];

inline double f(double x, int i) { return x * x * a[i] + x * b[i] + c[i]; }

inline double check(double x) {
  double ans = f(x, 1);
  for (int i = 2; i <= n; i++)
    ans = std::max(ans, f(x, i));
  return ans;
}

inline void solve() {
  std::cin >> n;
  for (int i = 1; i <= n; i++)
    std::cin >> a[i] >> b[i] >> c[i];
  double l = 0, r = 1000, emp = 1e-11, m1, m2;
  while (r - l > emp) {
    m1 = l + (r - l) / 3.0;
    m2 = r - (r - l) / 3.0;
    if (check(m1) > check(m2))
      l = m1;
    else
      r = m2;
  }
  printf("%.4lf\n", check(l));
}

int32_t main() {
  int T;
  std::cin >> T;
  while (T--) {
    solve();
  }
}