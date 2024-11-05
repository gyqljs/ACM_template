#include <bits/stdc++.h>

#define int long long

const long double pi = acosl(-1);
const int g = 3, gi = 332748118, p = 998244353;

inline int qpow(int b, int e) {
  int r = 1;
  while (e) {
    if (e & 1)
      r = r * b % p;
    b = b * b % p;
    e /= 2;
  }
  return r % p;
}

// using pii = std::pair<int, int>;
#define x first
#define y second

#define add(a, b) (a + b >= p ? a + b - p : a + b)
#define sub(a, b) (a - b >= 0 ? a - b : a - b + p)

using poly = std::vector<int>;


poly w, r;
int lim;


namespace rem {
  struct cp {
    int x, y;
  };

  int w;

  cp operator*(cp a, cp b) { // 复数乘法
    cp res;
    res.x = ((a.x * b.x % p + a.y * b.y % p * w % p) % p + p) % p; // x = a.x*b.x + a.y*b.y*w
    res.y = ((a.x * b.y % p + a.y * b.x % p) % p + p) % p; // y = a.x*b.y + a.y*b.x
    return res;
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
    if (n == 0)
      return 0;
    srand(time(0));
    n %= p;
    if (::qpow(n, (p - 1) / 2) == -1 + p)
      return -1;
    int a;
    while (1) {
      a = rand() % p;
      w = (((a * a) % p - n) % p + p) % p;
      if (::qpow(w, (p - 1) / 2) == -1 + p)
        break;
    }

    cp x = {a, 1};
    return qpow(x, (p + 1) / 2);
  }

  int calc(int n) {
    int ans = cipolla(n);
    int res = p - ans;
    return std::min(ans, res);
  }

}; // namespace rem

int64_t lagrange(poly &a, poly &b, int k) {
  int64_t res = 0;
  for (int i = 0; i < a.size(); ++i) {
    int64_t l = 1;
    for (int j = 0; j < a.size(); j++) {
      if (i == j) continue;
      l = 1ll * l * sub(k, a[j]) % p * qpow(sub(a[i], a[j]), p - 2) % p;
    }
    res = 1ll * (res + l * b[i]) % p;
  }
  return res;
}

void getr(int tn) {
  int cnt;
  lim = 1, cnt = 0;
  while (lim <= tn) lim <<= 1, cnt++;
  w.resize(lim), r.resize(lim);
  for (int i = 1; i < lim; i++)
    r[i] = (r[i >> 1] >> 1) | ((i & 1) << (cnt - 1));
}


void ntt(poly &a, int op) {
  for (int i = 0; i < lim; i++)
    if (r[i] > i)
      std::swap(a[r[i]], a[i]);
  w[0] = 1;
  for (int k = 1; k < lim; k <<= 1) {
    int Wn = qpow(op == 1 ? g : gi, (p - 1) / (k << 1));
    for (int i = 1; i < k; i++) w[i] = w[i - 1] * Wn % p;
    for (int i = 0; i < lim; i += k << 1) {
      for (int j = 0; j < k; j++) {
        int x = a[i + j], y = w[j] * a[i + j + k] % p;
        a[i + j] = add(x, y), a[i + j + k] = sub(x, y);
      }
    }
  }
  if (op != 1) {
    int invL = qpow(lim, p - 2);
    for (int i = 0; i < lim; i++) a[i] = a[i] * invL % p;
  }
}

poly operator+(const poly &a, const poly &b) {
  int num = std::max(a.size(), b.size());
  poly c(num);

  for (int i = 0; i < num; i++) {
    c[i] = add(a[i], b[i]);
  }
  return c;
}

poly operator-(const poly &a, const poly &b) {
  int num = std::max(a.size(), b.size());

  poly c(num);
  for (int i = 0; i < num; i++) {
    c[i] = sub(a[i], b[i]);
  }
  return c;
}

poly operator*(int a, poly b) {
  for (int &i: b)
    i = (i * a) % p;
  return b;
}

poly operator*(poly b, int a) {
  for (int &i: b)
    i = (i * a) % p;
  return b;
}

poly operator*(poly a, poly b) {
  int num = a.size() + b.size() - 1;
  getr(a.size() + b.size() - 1);
  a.resize(lim), b.resize(lim);
  ntt(a, 1), ntt(b, 1);
  for (int i = 0; i < lim; i++)
    a[i] = a[i] * b[i] % p;
  ntt(a, -1);
  a.resize(num);
  return a;
}

// 逆元
poly inv(poly a) {
  if (a.size() == 1)
    return a[0] = qpow(a[0], p - 2), a;
  int n = a.size();
  poly h = a;
  h.resize((n + 1) >> 1);
  poly b = inv(h);
  getr(2 * n);
  a.resize(lim), b.resize(lim);
  ntt(a, 1), ntt(b, 1);
  for (int i = 0; i < lim; i++)
    b[i] = b[i] * (p + 2 - a[i] * b[i] % p) % p;
  ntt(b, -1);
  b.resize(n);
  return b;
}

poly operator/(poly a, poly b) {
  int n = a.size(), m = b.size();
  std::reverse(a.begin(), a.end());
  std::reverse(b.begin(), b.end());
  b.resize(n - m + 1);
  b = inv(b);
  a = a * b;
  a.resize(n - m + 1);
  std::reverse(a.begin(), a.end());
  return a;
}

poly operator%(poly a, poly b) {
  poly ans = a - a / b * b;
  ans.resize(b.size() - 1);
  return ans;
}
// 求导
void d(poly &a) {
  int n = a.size();
  for (int i = 0; i < n - 1; i++)
    a[i] = (i + 1) * a[i + 1] % p;
  a[n - 1] = 0;
}

// 积分
void j(poly &a) {
  int n = a.size();
  for (int i = n - 1; i >= 1; i--)
    a[i] = qpow(i, p - 2) * a[i - 1] % p;
  a[0] = 0;
}

// 对数
poly ln(poly a) {
  poly tmp = a;
  d(tmp);
  int n = a.size();
  a = tmp * inv(a);
  j(a);
  a.resize(n);
  return a;
}

// 指数
poly exp(poly a) {
  if (a.size() == 1) return a[0] = 1, a;
  int n = a.size();
  poly h = a;
  h.resize((n + 1) >> 1);
  poly b = exp(h);
  b.resize(n);
  poly tb = ln(b);
  for (int i = 0; i < n; i++)
    tb[i] = (p + a[i] - tb[i]) % p;
  tb[0]++;
  b = b * tb;
  b.resize(n);
  return b;
}

// g0 = f0
poly dac_ntt(poly g) {
  for (int i = 1; i < g.size(); i++)
    g[i] *= -1;
  g[0] = 1;
  return inv(g);
}

poly pow(poly b, std::string &s) {
  int n = b.size();
  int j = n, k;
  for (int i = 0; i < n; i++)
    if (b[i]) {
      j = i;
      break;
    }
  if (j == n) return b;
  if (j) {
    if (s.size() > 8 || j * stoll(s) >= n) {
      fill_n(b.begin(), n, 0);
      return b;
    }
  }
  int m = 0, l = 0;
  for (auto c: s)
    m = (m * 10ll + c - '0') % p, l = (l * 10ll + c - '0') % (p - 1);
  for (int i = 0; i < n - j; i++)
    b[i] = b[i + j];
  for (int i = n - j; i < n; i++)
    b[i] = 0;
  k = b[0];
  assert(k);
  b = exp(ln(b * qpow(k, p - 2)) * m) * qpow(k, l);
  j *= m;
  for (int i = n - 1; i >= j; i--)
    b[i] = b[i - j];
  for (int i = 0; i < j; i++)
    b[i] = 0;
  return b;
}

poly sqrt(poly a) {
  if (a.size() == 1) return poly(1, rem::calc(a[0]));
  const int len = a.size();
  a.resize((len + 1) >> 1);
  poly b = sqrt(a);
  b.resize(len);
  auto c = b * b;
  c.resize(len);
  for (auto &it: b) it = 2ll * it % p;
  for (int i = 0; i < len; i++)
    c[i] = (c[i] + a[i]) % p;
  c = c * inv(b), c.resize(len);
  return c;
}

poly berlekampMassey(poly &s) {
  poly c, oldC;
  int f = -1;
  for (int i = 0; i < s.size(); i++) {
    auto delta = s[i];
    for (int j = 1; j <= c.size(); j++) {
      delta -= c[j - 1] * s[i - j];
    }
    if (delta == 0) {
      continue;
    }
    if (f == -1) {
      c.resize(i + 1);
      f = i;
    } else {
      auto d = oldC;
      d = d * -1;
      d.insert(d.begin(), 1);
      int df1 = 0;
      for (int j = 1; j <= d.size(); j++) {
        df1 += d[j - 1] * s[f + 1 - j] % p;
      }
      assert(df1 != 0);
      auto coef = delta / df1;
      d = d * coef;
      std::vector<int> z(i - f - 1);
      z.insert(z.end(), d.begin(), d.end());
      d = z;
      auto temp = c;
      c = c + d;
      if (i - temp.size() > f - oldC.size()) {
        oldC = temp;
        f = i;
      }
    }
  }
  c = c * -1;
  c.insert(c.begin(), 1);
  return c;
}

auto linearRecurrence(poly p, poly q, size_t n) {
  int m = q.size() - 1;
  while (n > 0) {
    auto newq = q;
    for (int i = 1; i <= m; i += 2) {
      newq[i] *= -1;
    }
    auto newp = p * newq;
    newq = q * newq;
    for (int i = 0; i < m; i++) {
      p[i] = newp[i * 2 + n % 2];
    }
    for (int i = 0; i <= m; i++) {
      q[i] = newq[i * 2];
    }
    n /= 2;
  }
  return p[0] / q[0];
}

// 任意模数
struct C {
  long double x, y;
  explicit C(long double a = 0, long double b = 0): x(a), y(b) {}
  C operator+(const C &b) { return C(x + b.x, y + b.y); }
  C operator-(const C &b) { return C(x - b.x, y - b.y); }
  C operator*(const C &b) { return C(x * b.x - y * b.y, x * b.y + y * b.x); }
  C operator/(long double b) { return C(x / b, y / b); }
};

void fft(std::vector<C> &a, int op) {
  for (int i = 0; i < lim; i++)
    if (r[i] > i)
      std::swap(a[i], a[r[i]]);
  for (int k = 1; k < lim; k <<= 1) {
    C Wn = C(cosl(pi / k), op * sinl(pi / k));
    for (int i = 0; i < lim; i += k << 1) {
      C w(1, 0), tmp;
      for (int j = i; j < i + k; j++, w = w * Wn)
        tmp = a[j + k] * w, a[j + k] = a[j] - tmp, a[j] = a[j] + tmp;
    }
  }
  if (op < 0)
    for (int i = 0; i < lim; i++) a[i] = a[i] / lim;
}

poly mul(const poly &a, const poly &b, int p) {
  int n = a.size(), m = b.size(), s = n + m - 1;
  getr(s);
  std::vector<C> c(lim), d(lim), e(lim);
  for (int i = 0; i < n; i++)
    c[i] = C(a[i] >> 15, a[i] & INT16_MAX);
  fill(c.begin() + n, c.begin() + lim, C());
  for (int i = 0; i < m; i++)
    d[i] = C(b[i] >> 15, b[i] & INT16_MAX);
  fill(d.begin() + m, d.begin() + lim, C());
  fft(c, 1), fft(d, 1);
  for (int i = 0; i < lim; i++) {
    int j = (lim - i) & (lim - 1);
    e[i] = d[i] * C(0.5 * (c[i].x + c[j].x), 0.5 * (c[i].y - c[j].y));
    d[i] = d[i] * C(0.5 * (c[i].y + c[j].y), 0.5 * (c[j].x - c[i].x));
  }
  fft(e, -1), fft(d, -1);
  poly ret(s);
  for (int i = 0; i < s; i++) {
    int64_t x = roundl(e[i].x), y = roundl(e[i].y), z = roundl(d[i].x), w = roundl(d[i].y);
    ret[i] = (((x % p << 30) % p + ((y + z) % p << 15) % p + w % p) % p + p) % p;
  }
  return ret;
}

// 卡特兰数
// H[n] = C(2 * n, n)
// H[n] = (4 * n - 2) * H[n - 1] / (n + 1)
