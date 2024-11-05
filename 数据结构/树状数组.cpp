#include <bits/stdc++.h>

// add(x, k) add(y + 1, -k)
template<class T>
struct fenw {
private:
  int n;
  std::vector<T> s;

public:
  void add(int p, int x) {
    while (p <= n) {
      s[p] += x;
      p += p & -p; // 因为是添加应该从下至上,故不停+lowbit
    }
  }

  int query(int p) {
    int ans = 0;
    while (p > 0) {
      ans += s[p];
      p -= p & -p; // 因为是查询所以应该从上至下，故不停地-lowbit 进行统计
    }
    return ans;
  }

  int query(int l, int r) {
    return query(r) - query(l - 1);
  }

  int select(const T &k) {
    int x = 0;
    T cur{};
    for (int i = 1 << (int) std::log2(n); i; i /= 2) {
      if (x + i <= n and cur + s[x + i - 1] <= k) {
        x += i;
        cur = cur + s[x - 1];
      }
    }
    return x;
  }

  explicit fenw(int n = 0): n(n - 1), s(n) {}

  explicit fenw(std::vector<T> &a): n(a.size() - 1), s(a) {
    for (int i = 1; i <= n; ++i) {
      int j = i + (i & -i);
      if (j <= n)
        s[j] += s[i];
    }
  }
};

// 二维树状数组
struct fenw2d {
  int n, m;
  std::vector<std::vector<int>> b1, b2, b3, b4;

  fenw2d(int n, int m): n(n), m(m) {
    b1.resize(n + 1, std::vector<int>(m + 1));
    b2.resize(n + 1, std::vector<int>(m + 1));
    b3.resize(n + 1, std::vector<int>(m + 1));
    b4.resize(n + 1, std::vector<int>(m + 1));
  }
  void change(auto &w, int x, int y, int k) { // 单点修改
    for (int i = x; i <= n; i += i & -i) {
      for (int j = y; j <= m; j += j & -j) {
        w[i][j] += k;
      }
    }
  }

  void change(int x, int y, int k) { // 多了一步计算
    change(b1, x, y, k);
    change(b2, x, y, k * (x - 1));
    change(b3, x, y, k * (y - 1));
    change(b4, x, y, k * (x - 1) * (y - 1));
  }

  void add(int x, int y, int X, int Y, int k) { // 区块修改：二维差分
    X++, Y++;
    change(x, y, k), change(X, y, -k);
    change(X, Y, k), change(x, Y, -k);
  }

  int query(auto &w, int x, int y) {
    int ans = 0;
    for (int i = x; i; i -= i & -i) {
      for (int j = y; j; j -= j & -j) {
        ans += w[i][j];
      }
    }
    return ans;
  }

  int query(int x, int y) {
    int ans = 0;
    ans += x * y * query(b1, x, y);
    ans -= y * query(b2, x, y);
    ans -= x * query(b3, x, y);
    ans += query(b4, x, y);
    return ans;
  }

  int ask(int x, int y, int X, int Y) {
    x--, y--;
    return query(X, Y) - query(x, Y) - query(X, y) + query(x, y);
  }
};


// 树状数组套权值线段树森林
template<typename T>
struct wseg_forest {
  struct node {
    int l = 0, r = 0;
    T v = 0;
  };
  int n, tot = 0;
  std::vector<node> tr;
  std::vector<int> root;

  explicit wseg_forest(int n, int sz = 1): n(n), tr(100 * n), root(sz + 1) {}

  void change_(int &u, int l, int r, int p, int k) { // 点修
    if (!u) {
      u = ++tot; // 开点
    }
    tr[u].v += k;
    if (l == r) return;
    int m = (l + r) >> 1;
    if (p <= m)
      change_(tr[u].l, l, m, p, k);
    else
      change_(tr[u].r, m + 1, r, p, k);
  }

  int kth_(std::vector<int> &lf, std::vector<int> &rf, int l, int r, int k) {
    if (l == r)
      return l;
    int m = (l + r) >> 1;
    int64_t s = 0;
    for (int u: lf) s -= tr[tr[u].l].v;
    for (int u: rf) s += tr[tr[u].l].v;
    if (s >= k) {
      for (int &u: lf) u = tr[u].l;
      for (int &u: rf) u = tr[u].l;
      return kth_(lf, rf, l, m, k);
    } else {
      for (int &u: lf) u = tr[u].r;
      for (int &u: rf) u = tr[u].r;
      return kth_(lf, rf, m + 1, r, k - s);
    }
  }

  void add(int p, int v) {
    while (p < root.size()) {
      change_(root[p], 1, n, v, 1);
      p += p & -p;
    }
  }

  void sub(int p, int v) {
    while (p < root.size()) {
      change_(root[p], 1, n, v, -1);
      p += p & -p;
    }
  }

  int kth(int l, int r, int k) {
    std::vector<int> lf, rf;
    l--;
    while (l > 0) {
      lf.push_back(root[l]);
      l -= l & -l;
    }
    while (r > 0) {
      rf.push_back(root[r]);
      r -= r & -r;
    }
    return kth_(lf, rf, 1, n, k);
  }
};