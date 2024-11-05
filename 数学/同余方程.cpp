#include <bits/stdc++.h>

// 乘法 a * b % c
int mul(int a, int b, int c) {
  int res = 0;
  while (b > 0) {
    if (b & 1) {
      res = ((res + a)) % c;
    }
    a = (a + a) % c;
    b >>= 1;
  }
  return (res % c + c) % c;
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

// 中国剩余定理
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
        int bei = (l - r) / x;
        bei = std::max(bei, 1);
        r += bei * x;
      }
    }
    x = (x * p[i]) / std::gcd(x, p[i]);
    y = r % x;
  }
  return y;
}


// 拓展中国剩余定理
int excrt(std::vector<int> &a, std::vector<int> &b) {
  int ans, x, y;
  int m = b[0];
  ans = a[0];
  for (int i = 1; i < a.size(); i++) {
    int c = ((a[i] - ans) % b[i] + b[i]) % b[i];
    int g = exgcd(m, b[i], x, y);
    int p = b[i] / g;
    x = mul(x, c / g, p);
    ans += x * m;
    m *= p;
    ans = (ans % m + m) % m;
  }
  return ans % m;
}

// 求ax+by=c的正整数解数量，x y最小以及最大的正整数
int ji_num(int a, int b, int c) {
  std::vector<int> v;
  int x, y;
  int d = exgcd(a, b, x, y);
  if (c % d != 0) {
    // 无解
    return (int) 0;
  }
  a = a / d;
  b = b / d;
  c = c / d;
  x = x * c;
  y = y * c;
  int i = ceil(-1.0 * x / b);
  if (i == -1.0 * x / b)
    i++;
  int j = floor(1.0 * y / a);
  if (j == 1.0 * y / a)
    j--;
  if (i > j) {
    // 只有一组解 x + b * i , y - a * j
    return (int) 1;
  } else {
    // x, y的取值范围
    // x + b * i  y - a * j : x + b * j  y - a * i
    return (j - i + 1);
  }
}
namespace rem {
  struct cp {
    int x, y;
  };

  int w, p;

  cp operator*(cp a, cp b) { // 复数乘法
    cp res;
    res.x = ((a.x * b.x % p + a.y * b.y % p * w % p) % p + p) % p; // x = a.x*b.x + a.y*b.y*w
    res.y = ((a.x * b.y % p + a.y * b.x % p) % p + p) % p; // y = a.x*b.y + a.y*b.x
    return res;
  }

  int qpow(int b, int e) {
    int r = 1;
    while (e) {
      if (e & 1)
        r = r * b % p;
      b = b * b % p;
      e /= 2;
    }
    return r % p;
  }

  int qpow(cp b, int e) {
    cp r = {1, 0};
    while (e) {
      if (e & 1)
        r = r * b;
      b = b * b;
      e /= 2;
    }
    return r.x;
  }

  // n模p的二次剩余
  int cipolla(int n) {
    srand(time(0));
    n %= p;
    if (qpow(n, (p - 1) / 2) == -1 + p)
      return -1;
    int a;
    while (1) {
      a = rand() % p;
      w = (((a * a) % p - n) % p + p) % p;
      if (qpow(w, (p - 1) / 2) == -1 + p)
        break;
    }

    cp x = {a, 1};
    return qpow(x, (p + 1) / 2);
  }

}; // namespace rem