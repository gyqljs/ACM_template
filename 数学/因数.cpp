#include <bits/stdc++.h>

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

// 分解质因数

void fp(int n, std::unordered_map<int, int> &mp) {
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
}

auto fp(int n) {
  std::vector<int> v;
  if (n % 2 == 0)
    v.push_back(2);
  while (n % 2 == 0) {
    n = n / 2;
  }
  for (int i = 3; i <= sqrt(n); i = i + 2) {
    if (n % i == 0)
      v.push_back(i);
    while (n % i == 0) {
      n = n / i;
    }
  }
  if (n > 2)
    v.push_back(n);
  return v;
}

// 找到整数n的所有因数
void fc(int n, std::unordered_map<int, int> &mp) {
  for (int i = 1; i * i <= n; ++i) {
    if (n % i == 0) {
      mp[i]++;
      if (i != n / i) {
        mp[n / i]++;
      }
    }
  }
}

// 判断一个数是否为素数
bool isPrime(long long n) {
  if (n < 2) return false;
  for (long long i = 2; i * i <= n; i++) {
    if (n % i == 0) return false;
  }
  return true;
}

// 寻找p的一个原根，若p不是素数则返回-1
long long findPrimitiveRoot(long long p) {
  if (!isPrime(p)) {
    // 可以扩展支持更多情况，如 p = 2, 4, p^k, 2*p^k 等
    // 这里只处理素数情况
    return -1;
  }

  // 获取p-1的所有唯一素因子
  long long phi = p - 1;
  // std::vector<int>
  std::vector<int> factors = fp(phi);

  // 从2开始寻找原根
  for (long long g = 2; g < p; g++) {
    bool flag = true;
    for (auto &factor: factors) {
      if (qpow(g, phi / factor, p) == 1) {
        flag = false;
        break;
      }
    }
    if (flag) {
      return g;
    }
  }
  return -1; // 理论上，对于素数p，总有原根
}


void fc(int n, std::vector<int> &v) {
  for (int i = 1; i * i <= n; ++i) {
    if (n % i == 0) {
      v.push_back(i);
      if (i != n / i) {
        v.push_back(n / i);
      }
    }
  }
}