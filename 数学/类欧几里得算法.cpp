#include <bits/stdc++.h>


#define int long long

const int p = 998244353, i2 = 499122177, i6 = 166374059;

struct data {
  int f, g, h;
  data() { f = g = h = 0; }
};

data calc(int n, int a, int b, int c) {
  int ac = a / c, bc = b / c, m = (a * n + b) / c, n1 = n + 1, n21 = n * 2 + 1;
  data t;
  if (a == 0) {
    t.f = bc * n1 % p;
    t.g = bc * n % p * n1 % p * i2 % p;
    t.h = bc * bc % p * n1 % p;
    return t;
  }
  if (a >= c || b >= c) {
    t.f = n * n1 % p * i2 % p * ac % p + bc * n1 % p;
    t.g = ac * n % p * n1 % p * n21 % p * i6 % p + bc * n % p * n1 % p * i2 % p;
    t.h = ac * ac % p * n % p * n1 % p * n21 % p * i6 % p + bc * bc % p * n1 % p + ac * bc % p * n % p * n1 % p;
    t.f %= p, t.g %= p, t.h %= p;

    data e = calc(n, a % c, b % c, c);

    t.h += e.h + 2 * bc % p * e.f % p + 2 * ac % p * e.g % p;
    t.g += e.g, t.f += e.f;
    t.f %= p, t.g %= p, t.h %= p;
    return t;
  }
  data e = calc(m - 1, c, c - b - 1, a);
  t.f = n * m % p - e.f, t.f = (t.f % p + p) % p;
  t.g = m * n % p * n1 % p - e.h - e.f, t.g = (t.g * i2 % p + p) % p;
  t.h = n * m % p * (m + 1) % p - 2 * e.g - 2 * e.f - t.f;
  t.h = (t.h % p + p) % p;
  return t;
}
