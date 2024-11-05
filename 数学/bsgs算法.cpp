#include <bits/stdc++.h>

// p是质数
int bsgs(int a, int b, int p) {
  a %= p, b %= p;
  if (b == 1)
    return 0;
  int m = ceil(sqrt(p));
  int t = b;
  std::unordered_map<int, int> mp;
  mp[b] = 0;
  for (int j = 1; j < m; j++) {
    t = t * a % p; // 求b*a^j
    mp[t] = j;
  }

  int mi = 1;
  for (int i = 1; i <= m; i++)
    mi = mi * a % p; // 求a^m
  t = 1;
  for (int i = 1; i <= m; i++) {
    t = t * mi % p; // 求(a^m)^i
    if (mp.count(t))
      return i * m - mp[t];
  }
  return -1; // 无解
}

int exbsgs(int a, int b, int p) {
  a %= p;
  b %= p;
  if (b == 1 || p == 1) return 0; // x=0

  int d, k = 0, x = 1;
  while (true) {
    d = std::gcd(a, p);
    if (d == 1) break;
    if (b % d)
      return -1; // 无解
    k++;
    b /= d;
    p /= d;
    x = x * (a / d) % p; // 求a^k/D
    if (x == b) return k;
  }

  int m = ceil(sqrt(p));
  int t = b;
  std::unordered_map<int, int> mp;
  mp[b] = 0;
  for (int j = 1; j < m; j++) {
    t = t * a % p; // 求b*a^j
    mp[t] = j;
  }
  int mi = 1;
  for (int i = 1; i <= m; i++)
    mi = mi * a % p; // 求a^m
  t = x;
  for (int i = 1; i <= m; i++) {
    t = t * mi % p; // 求(a^m)^i
    if (mp.count(t))
      return i * m - mp[t] + k;
  }
  return -1; // 无解
}