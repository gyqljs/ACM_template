#include <bits/stdc++.h>

int qpow(int b, int e, int m) {
  int r = 1;
  while (e) {
    if (e & 1)
      r = r * b % m;
    b = b * b % m;
    e /= 2;
  }
  return r % m;
}

// 拓展欧几里得算法
int exgcd(int a, int b, int &x, int &y) {
  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  }
  int r = exgcd(b, a % b, x, y);
  int t = y;
  y = x - (a / b) * y;
  x = t;
  return r;
}

int crt(std::vector<int> &a, std::vector<int> &p) {
  int x = 0, y = 0;
  for (int i = 0; i < a.size(); i++) {
    if (x == 0 && y == 0) {
      x = p[i];
      y = a[i];
      continue;
    }
    int l = a[i], r = y;
    while (l != r) {
      if (l < r) {
        int b = (r - l) / p[i];
        b = std::max(b, 1);
        l += b * p[i];
      } else {
        int b = (l - r) / x;
        b = std::max(b, 1);
        r += b * x;
      }
    }
    x = (x * p[i]) / std::gcd(x, p[i]);
    y = r % x;
  }
  return y;
}


// n的逆元
int inv(int a, int p) {
  int x, y;
  int gcd = exgcd(a, p, x, y);
  if (gcd != 1) {
    return -1;
  }
  return (x % p + p) % p;
}

const int p = 999911659;
int farc[50010];
void init(int p) {
  farc[0] = 1;
  for (int i = 1; i <= p; i++) {
    farc[i] = farc[i - 1] * i % p;
  }
}

int comb(int n, int m, int p) {
  if (n < m) return 0;
  return farc[n] * qpow(farc[m], p - 2, p) % p * qpow(farc[n - m], p - 2, p) % p;
}


int lucas(int n, int m, int p) {
  if (m == 0) return 1;
  return lucas(n / p, m / p, p) * comb(n % p, m % p, p) % p;
}


// pk表示p的k次幂 计算n! % pow(p, pk);
int fac_mod(int n, int p, int pk) {
  if (!n) return 1;
  int res = 1;
  // 一个循环节内的乘积, 周期为pk
  for (int i = 1; i <= pk; i++)
    if (i % p)
      res = res * i % pk;
  // n/pk个完整周期
  res = qpow(res, n / pk, pk);
  // 不满足一个周期的数的乘积
  for (int i = 1; i <= n % pk; i++)
    if (i % p)
      res = res * i % pk;
  // 递归求 (n/p)!除去质因子p并对p的k次幂取模
  return res * fac_mod(n / p, p, pk) % pk;
}


// 组合数c(n, m)对p的pk次幂取模
int C_mod(int n, int m, int p, int pk) {
  int x, y, z;
  x = y = z = 0;
  // 最后乘p的(x-y-z)次幂
  // n!中p质因子的数目
  for (int i = p; i <= n; i *= p) x += n / i;
  // m!中p质因子的数目
  for (int i = p; i <= m; i *= p) y += m / i;
  //(n-m)!中p质因子的数目
  for (int i = p; i <= n - m; i *= p) z += (n - m) / i;
  //-------------------------------
  // n!/m!/(n-m)!
  // 求两次逆元
  // 三次阶乘除以质因子模质数幂
  return fac_mod(n, p, pk) * inv(fac_mod(m, p, pk), pk) % pk * inv(fac_mod(n - m, p, pk), pk) % pk * qpow(p, x - y - z, pk);
}

int ex_lucas(int n, int m, int p) {
  std::vector<int> a, b;
  for (int i = 2; i * i <= p; i++) {
    if (p % i == 0) {
      int pk = 1;
      while (p % i == 0) {
        pk *= i;
        p /= i;
      }

      a.push_back(pk);
      b.push_back(C_mod(n, m, i, pk));
    }
  }
  if (p > 1) {
    a.push_back(p);
    b.push_back(C_mod(n, m, p, p));
  }
  return crt(b, a);
}



// 卢卡斯定理 c(n, m) % p = c(n / p,m / p) * c(n % p, m % p) % p;


// 威尔逊定理 对于质数p * (p - 1)! == -1 (mod p)