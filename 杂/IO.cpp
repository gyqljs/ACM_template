#ifdef ONLINE_JUDGE
#pragma GCC optimize("O3")
#endif

#include <bits/stdc++.h>

const int inf = 0x3f3f3f3f;

// 常见模数以及原根
// 167772161 3
// 469762049 3
// 754974721 11
// 998244353 3
// 1004535809 3

// 快速幂
int pow(int b, int e, int m = INT32_MAX) {
  int r = 1;
  while (e) {
    if (e & 1)
      r = r * b % m;
    b = b * b % m;
    e /= 2;
  }
  return r % m;
}


// 可以使用 __int128
std::istream &operator>>(std::istream &is, __int128 &x) {
  x = 0;
  bool neg = false;
  char ch = getchar();
  while (ch != EOF && (ch < '0' || ch > '9')) {
    if (ch == '-') {
      neg = true;
    }
    ch = getchar();
  }
  while (ch >= '0' && ch <= '9') {
    x = x * 10 + (ch - '0');
    ch = getchar();
  }
  if (neg) {
    x = -x;
  }
  return is;
}

std::ostream &operator<<(std::ostream &os, __int128 x) {
  if (x < 0) {
    putchar('-');
    x = -x;
  }
  if (x > 9) {
    os << x / 10;
  }
  os << static_cast<char>('0' + x % 10);
  return os;
}

using int128_t = __int128;

// 快读 读入的同时取模
template<class T = int>
inline T read(T m = std::numeric_limits<T>::max()) {
  T x = 0, f = 1;
  char ch = getchar();
  while (!isdigit(ch)) {
    if (ch == '-')
      f = -1;
    ch = getchar();
  }
  while (isdigit(ch)) {
    x = (x << 1) + (x << 3) + (ch ^ 48);
    x %= m;
    ch = getchar();
  }
  return x * f % m;
}

template<class T = int>
inline T read(const std::string &s, T m = std::numeric_limits<T>::max()) {
  T x = 0;
  int f = 1;
  size_t i = 0;

  if (s[i] == '-') {
    f = -1;
    i++;
  }

  while (i < s.size() && !isdigit(s[i])) {
    i++;
  }

  while (i < s.size() && isdigit(s[i])) {
    x = (x * 10 + (s[i] - '0')) % m;
    i++;
  }

  return (x * f) % m;
}


// 数组模拟vector
template<class T, size_t N = 200005>
struct vector {
  T arr[N];
  int tot = 0;
  inline size_t size() { return tot; }
  inline void push_back(T u) { arr[tot++] = u; }
  inline T &back() { return arr[tot - 1]; }
  inline T &operator[](int id) { return arr[id]; }
  inline T *begin() { return arr; }
  inline T *end() { return arr + tot; }
  inline void clear() { tot = 0; }
  inline void pop_back() { tot--; }
  inline void resize(int n) { tot = n; }
};

// 二分搜索
template<class T, class F>
std::pair<int, int> findRange(const std::vector<F> &v, T a, T b) {
  // 使用 lower_bound 查找第一个 >= a 的元素
  auto it_l = std::lower_bound(v.begin(), v.end(), a);

  // 使用 upper_bound 查找第一个 > b 的元素
  auto it_r = std::upper_bound(v.begin(), v.end(), b);

  int l = (it_l != v.end()) ? std::distance(v.begin(), it_l) : -1;
  int r = (it_r != v.begin()) ? std::distance(v.begin(), it_r) - 1 : -1;

  if (l <= r && l < v.size() && r >= 0) {
    return {l, r};
  } else {
    return {-1, -1};
  }
}


int32_t main() {
  // 本地且文件非空读入文件
  std::ifstream file("text.in");
  if (file.is_open() and !file.eof())
    freopen("text.in", "r", stdin);
  // 关闭流同步
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
}