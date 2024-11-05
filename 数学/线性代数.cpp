#include <bits/stdc++.h>

using matrix = std::vector<std::vector<int>>;

// 快速幂
template<class T>
T qpow(T b, T e, T m = INT32_MAX) {
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

// n的逆元
int inv(int a, int p) {
  int x, y;
  int gcd = exgcd(a, p, x, y);
  if (gcd != 1) {
    return -1;
  }
  return (x % p + p) % p;
}

// 线性基
// int k = 0;
std::vector<int64_t> gauss(std::vector<int64_t> p, int &k) {
  // int k = 0;
  for (int i = 63; i >= 0; i--) {
    // 把当前第i位是1的数换上去
    for (int j = k; j < p.size(); j++)
      if (p[j] >> i & 1) {
        std::swap(p[j], p[k]);
        break;
      }
    if ((p[k] >> i & 1) == 0)
      continue;
    for (int j = 0; j < p.size(); j++)
      if (j != k && (p[j] >> i & 1)) p[j] ^= p[k];
    k++;
    if (k == p.size()) break;
  }
  return p;
}


// 矩阵快速幂
auto qp(matrix &v, int e, int m) {
  auto mul = [&](matrix &a, matrix &b) {
    int n = a.size();
    matrix c(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
        for (int l = 0; l < n; l++) {
          c[i][j] += a[i][l] % m * b[l][j];
          c[i][j] %= m;
        }
    return c;
  };

  auto qpow = [&](matrix &b, int e) {
    int n = b.size();
    matrix r(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
      r[i][i] = 1;
    }
    while (e) {
      if (e & 1)
        r = mul(r, b);
      b = mul(b, b);
      e /= 2;
    }
    return r;
  };
  return qpow(v, e);
}

//  求矩阵的值
int value(int mod, matrix mat) {
  int k, d, w = 1, n = mat.size();
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      while (mat[i][i]) {
        d = mat[j][i] / mat[i][i];
        for (k = i; k <= n; ++k)
          mat[j][k] = ((mat[j][k] - d * mat[i][k] % mod) % mod + mod) % mod;
        swap(mat[i], mat[j]);
        w = -w;
      }
      swap(mat[i], mat[j]);
      w = -w;
    }
  }
  int ret = w;
  for (int i = 0; i < n; ++i)
    ret = ret * mat[i][i] % mod;
  return (ret + mod) % mod;
}

// 高斯消元解方程组
void guass(std::vector<std::vector<double>> &a) {
  int n = a.size();
  int nw = 0;
  // k 指主元序号（列）
  for (int k = 0; k < n; ++k) { // 需要考虑无穷解，循环到N
    int m = nw;
    for (int i = nw + 1; i < n; ++i)
      if (abs(a[i][k]) > abs(a[m][k]))
        m = i;
    if (abs(a[m][k] - 0) < 1e-9)
      continue;
    for (int j = 0; j < n + 1; j++)
      std::swap(a[nw][j], a[m][j]);
    for (int i = 0; i < n; i++) {
      if (i == nw)
        continue;
      double mul = a[i][k] / a[nw][k];
      for (int j = k; j < n + 1; j++)
        a[i][j] -= a[nw][j] * mul;
    }
    nw++;
  }

  // 打印答案
  //  if (nw < n) { // 存在找不到主元的情况
  //      while (nw < n)
  //          if (!(abs(a[nw++][n] - 0) < 1e-9)) {
  //              puts("-1");
  //              return;
  //          }
  //      putchar('0');
  //  } else
  //      for (int i = 0; i < n; ++i)
  //          printf("x%d=%.2lf\n", i + 1, a[i][n] / a[i][i]);
}

// 求矩阵的逆
void gauss_jordan(std::vector<std::vector<int>> &a, int mod) {
  int n = a.size();
  for (int i = 0; i < n; ++i) {
    int r = i;
    for (int j = i + 1; j < n; ++j)
      if (a[j][i] > a[r][i])
        r = j;

    if (r != i)
      swap(a[i], a[r]);

    if (!a[i][i]) {
      puts("No Solution");
      return;
    }

    int x = qpow(a[i][i], mod - 2);
    for (int k = 0; k < n; ++k) {
      if (k == i) continue;
      int fc = inv(a[k][i], mod);
      for (int j = i; j < (n * 2); ++j)
        a[k][j] = (a[k][j] - fc * a[i][j] % mod + mod) % mod;
    }

    for (int j = 0; j < 2 * n; j++)
      a[i][j] = a[i][j] * x % mod;
  }

  // 打印答案
  //  for (int i = 0; i < n; ++i) {
  //      for (int j = n; j < (n * 2); ++j)
  //          printf("%lld ", a[i][j]);
  //      printf("\n");
  //  }
}
