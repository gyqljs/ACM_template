#include <bits/stdc++.h>

// 并查集
class dsu {
public:
  int n;
  std::vector<int> f, rk, sz, p, e;

  explicit dsu(int n): n(n), f(n), rk(n), sz(n, 1), p(n), e(n) {
    for (int i = 0; i < n; ++i)
      f[i] = i;
  }

  int find(int x) {
    if (f[x] != x) {
      int old = f[x];
      f[x] = find(f[x]);
      rk[x] += rk[old];
    }
    return f[x];
  }

  // y作为x的爸爸
  void unite(int x, int y) {
    x = find(x), y = find(y);
    e[y]++;
    if (x != y) {
      f[x] = y;
      rk[x] = sz[y];
      p[y] |= x;
      sz[y] += sz[x];
      e[y] += e[x];
    }
  }

  int dis(int x, int y) {
    if (find(x) != find(y))
      return -1;
    else
      return abs(rk[x] - rk[y]) - 1;
  }

  bool same(int x, int y) {
    return find(x) == find(y);
  }

  bool F(int x) {
    return p[find(x)];
  }

  std::vector<std::vector<int>> groups() {
    std::vector<std::vector<int>> temp(n);

    for (int i = 0; i < n; i++) {
      temp[find(i)].push_back(i);
    }

    return temp;
  }
};


// 加权并查集
class w_dsu {
public:
  std::vector<int> p, rk;
  int cnt;

  explicit w_dsu(int n): p(n), rk(n, 0), cnt(n) {
    for (int i = 0; i < n; i++)
      p[i] = i;
  }

  int find(int x) {
    // 查询过程不断更新当前节点和父节点的关系
    if (p[x] != x) {
      int old = p[x];
      p[x] = find(p[x]);
      rk[x] += rk[old];
    }
    return p[x];
  }

  void unite(int x, int y) {
    x = find(x), y = find(y);
    // 按照关系合并 默认按照秩合并,rk[x]通常记录x和他祖先的关系
    if (x != y) {
      if (rk[x] < rk[y])
        std::swap(x, y);
      p[y] = x;
      if (rk[x] == rk[y])
        ++rk[x];
      --cnt;
    }
  }
};

class lfb: private w_dsu {
public:
  struct node {
    int l, r, dis, v;
  };
  std::vector<node> t;

  // 合并以x和y为根的堆
  int merge(int x, int y) {
    if (!x || !y) return x + y;
    if (t[x].v == t[y].v ? x > y : t[x].v > t[y].v)
      std::swap(x, y);
    t[x].r = merge(t[x].r, y);
    if (t[t[x].l].dis < t[t[x].r].dis)
      std::swap(t[x].l, t[x].r);
    t[x].dis = t[t[x].r].dis + 1;
    return x;
  }

  explicit lfb(int n): w_dsu(n), t(n + 1) {
    for (int i = 0; i <= n; ++i) {
      t[i].l = t[i].r = t[i].dis = 0;
    }
    t[0].dis = -1;
  }

  void link(int x, int y) {
    if (t[x].v == -1 or t[y].v == -1)
      return;
    x = find(x), y = find(y);
    if (x == y) return;
    p[x] = p[y] = merge(x, y);
  }

  int pop(int x) {
    if (t[x].v == -1)
      return -1;
    x = find(x);
    int ans = t[x].v;
    t[x].v = -1;
    p[t[x].l] = p[t[x].r] = p[x] = merge(t[x].l, t[x].r);
    return ans;
  }
};
