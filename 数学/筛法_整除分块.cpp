#include <bits/stdc++.h>

// 欧拉函数
int euler_phi(int n) {
  int ans = n;
  for (int i = 2; i * i <= n; i++)
    if (n % i == 0) {
      ans = ans / i * (i - 1);
      while (n % i == 0) n /= i;
    }
  if (n > 1) ans = ans / n * (n - 1);
  return ans;
}

// 莫比乌斯函数
int mu_phi(int n) {
  if (n == 1)
    return 1;
  std::unordered_map<int, int> mp;
  while (n % 2 == 0) {
    mp[2]++;
    n = n / 2;
  }
  for (int i = 3; i <= sqrt(n); i = i + 2) {
    while (n % i == 0) {
      mp[i]++;
      n = n / i;
    }
  }
  if (n > 2)
    mp[n]++;
  int ans = 1;
  for (auto it: mp) {
    if (it.second > 1)
      return 0;
    ans *= -1;
  }
  return ans;
}


// 1~n的所有素数
std::vector<int> euler_sieve(int n) {
  std::vector<int> v;
  std::vector<bool> is_prime(n + 1, true);
  is_prime[0] = is_prime[1] = false;
  for (int i = 2; i <= n; i++) {
    if (is_prime[i]) {
      v.push_back(i);
    }
    for (int j = 0; j < v.size() && i * v[j] <= n; j++) {
      is_prime[i * v[j]] = false;
      if (i % v[j] == 0) break;
    }
  }
  return v;
}

// 1~n的所有模p逆元
std::vector<int> mod_inverse_array(int n, int p) {
  std::vector<int> inv(n + 1, 0);
  inv[1] = 1;
  for (int i = 2; i <= n; i++) {
    inv[i] = (p - (1LL * (p / i) * inv[p % i]) % p) % p;
    if (inv[i] < 0) {
      inv[i] += p;
    }
  }
  return inv;
}

// 求1~n所有数字对应的欧拉函数
std::vector<int> euler(int n) {
  std::vector<int> phi(n + 1);
  phi[1] = 1;
  for (int i = 2; i <= n; i++) {
    if (!phi[i]) {
      for (int j = i; j <= n; j += i) {
        if (!phi[j]) phi[j] = j;
        phi[j] = phi[j] / i * (i - 1);
      }
    }
  }
  return phi;
}

// 1~n所有整数对应的莫比乌斯函数
std::vector<int> getmiu(int n) {
  std::vector<bool> ip(n, 1);
  std::vector<int> p, mu(n + 1);
  mu[1] = 1;
  for (int i = 2; i <= n; i++) {
    if (ip[i])
      p.push_back(i), mu[i] = -1;
    for (int j: p) {
      if (i * j > n) break;
      ip[i * j] = 0;
      if (i % j == 0) {
        mu[i * j] = 0; // 略有不同
        break;
      } else
        mu[i * j] = -mu[i];
    }
  }
  return mu;
}

// 1~n的全部整数的最小质因子
void Eular(int n, std::vector<int> &lst) {
  int cnt = 0;
  std::vector<int> vis(n + 1), sp(n + 1);
  lst.resize(n + 1);
  vis[1] = true;
  lst[1] = 1;
  for (int i = 2; i <= n; i++) {
    if (!vis[i]) {
      sp[++cnt] = i;
      lst[i] = i;
    }
    for (int j = 1, p = sp[1]; j <= cnt && i * p <= n; p = sp[++j]) {
      vis[i * p] = true;
      lst[i * p] = p;
      if (i % p == 0) break;
    }
  }
  // 遍历
  // for (int p = lst[y]; p != 1; p = lst[y /= p]) //p就是y的所有质因子
}

// 整除分块
// k为定值,n为i的取值范围
int fen_kuai(int n, int k, std::vector<int> &f, std::vector<int> &g) {
  int ans = 0;
  for (int l = 1, r; l <= std::min(n, k); l = r + 1) {
    r = std::min(k / (k / l), n);
    // 二维的换成 r = std::min(n / (n / l), m / (m / l));
    ans += (f[r] - f[l - 1]) * g[k / l] * (r - l + 1);
    // 该部分代表左端点为l，右端点为r的区间:对于任意的l<=i<=r,有:n / i = k / l
  }
  return ans;
  //
}


// 杜教筛

// 莫比乌斯函数卷 g(x) = 1 的答案为E[x] = (x == 1)
// 欧拉函数卷 g(x) = 1 的答案为id[x] = x
// 莫比乌斯函数卷 id(x) = 1 的答案为欧拉函数

int N;

std::unordered_map<int, int> mp_mu, mp_phi;
// 先预处理要求函数的前缀和
std::vector<int> phi, mu;

int Sphi(int n) {
  if (n < N) return phi[n]; // 到达范围直接返回
  if (mp_phi[n]) return mp_phi[n]; // 找到过的直接返回
  int ans = n * (n + 1) / 2; // SUM(f * g) (i) 的答案
  for (int l = 2, r; l <= n; l = r + 1) {
    r = n / (n / l);
    ans -= Sphi(n / l) * (r - l + 1);
  }
  return mp_phi[n] = ans;
}

int Smu(int n) {
  if (n < N) return mu[n];
  if (mp_mu[n]) return mp_mu[n];
  int ans = 1;
  for (int l = 2, r; l <= n; l = r + 1) {
    r = n / (n / l);
    ans -= Smu(n / l) * (r - l + 1);
  }
  return mp_mu[n] = ans;
}
