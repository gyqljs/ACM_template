#include <bits/stdc++.h>


#define int long long

int quick_pow(int x, int p, int mod) { // 快速幂
  int ans = 1;
  while (p) {
    if (p & 1) ans = (__int128) ans * x % mod;
    x = (__int128) x * x % mod;
    p >>= 1;
  }
  return ans;
}

bool Miller_Rabin(int p) { // 判断素数
  if (p < 2) return 0;
  if (p == 2) return 1;
  if (p == 3) return 1;
  int d = p - 1, r = 0;
  while (!(d & 1)) ++r, d >>= 1; // 将d处理为奇数
  for (int k = 0; k < 10; ++k) {
    int a = rand() % (p - 2) + 2;
    int x = quick_pow(a, d, p);
    if (x == 1 || x == p - 1)
      continue;
    for (int i = 0; i < r - 1; ++i) {
      x = (__int128) x * x % p;
      if (x == p - 1) break;
    }
    if (x != p - 1) return 0;
  }
  return 1;
}

int Pollard_Rho(int x) {
  int s = 0, t = 0;
  int c = (int) rand() % (x - 1) + 1;
  int step = 0, goal = 1;
  int val = 1;
  while (1) { // 倍增优化
    for (step = 1; step <= goal; ++step) {
      t = ((__int128) t * t + c) % x;
      val = (__int128) val * abs(t - s) % x;
      if ((step % 127) == 0) {
        int d = std::gcd(val, x);
        if (d > 1)
          return d;
      }
    }
    int d = std::gcd(val, x);
    if (d > 1)
      return d;
    goal *= 2, s = t, val = 1;
  }
}

int fac(int x, int ans = 0) {
  if (x <= ans || x < 2)
    return ans;
  if (Miller_Rabin(x)) {
    return std::max(ans, x);
  }
  int p = x;
  while (p >= x)
    p = Pollard_Rho(x);
  while ((x % p) == 0)
    x /= p;
  return std::max(fac(x, ans), fac(p, ans));
}