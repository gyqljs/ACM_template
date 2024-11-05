#include <bits/stdc++.h>

using pii = std::pair<int, int>;
#define x first
#define y second

struct tree {
  int n, tot = 1;
  std::vector<std::array<int, 22>> val, rmq;
  std::vector<int> lg, dep, dfn, pre;

  int op(int a, int b) { return std::min(a, b); }

  explicit tree(std::vector<std::vector<pii>> &e, int rt = 1): n(e.size()), lg(n), dep(n), val(n), rmq(n), dfn(n), pre(n) {
    n--;
    for (int i = 1; i <= n; i++) { // 预处理 log
      lg[i] = lg[i - 1] + (1 << lg[i - 1] == i);
    }
    std::function<void(int, int)> dfs = [&](int x, int fa) {
      val[x][0] = fa;
      dep[x] = dep[fa] + 1;
      // 虚树拓展
      dfn[x] = ++tot;
      for (int i = 1; i <= lg[dep[x]]; i++) {
        // 最近公共祖先查询拓展
        val[x][i] = val[val[x][i - 1]][i - 1];
        // rmq查询拓展
        rmq[x][i] = op(rmq[x][i - 1], rmq[val[x][i - 1]][i - 1]);
      }
      for (auto [y, w]: e[x]) {
        if (y == fa)
          continue;
        rmq[y][0] = w;
        // 前缀和拓展
        pre[y] += pre[x] + w;
        dfs(y, x);
      }
    };
    dfs(rt, 0);
  }

  int operator[](int id) { return val[id][0]; }

  int lca(int x, int y) {
    if (dep[x] < dep[y])
      std::swap(x, y);
    while (dep[x] > dep[y]) {
      x = val[x][lg[dep[x] - dep[y]] - 1];
    }
    if (x == y)
      return x;
    for (int k = lg[dep[x]] - 1; k >= 0; k--) {
      if (val[x][k] == val[y][k])
        continue;
      x = val[x][k];
      y = val[y][k];
    }
    return val[x][0];
  }

  int clac(int x, int y) { // 倍增查询两点间距离
    return dep[x] + dep[y] - 2 * dep[lca(x, y)];
  }

  int dis(int x, int y) {
    return pre[x] + pre[y] - 2 * pre[lca(x, y)];
  }

  int get(int x, int y) {
    int ans = 2e9;
    if (x == y)
      return ans;
    for (int i = lg[dep[x]]; i >= 0; i--) {
      if (dep[val[x][i]] > dep[y]) {

        ans = op(ans, rmq[x][i]);
        x = val[x][i];
      }
    }
    ans = op(ans, rmq[x][0]);
    return ans;
  }

  int query(int x, int y) {
    int fa = lca(x, y);
    return op(get(x, fa), get(y, fa));
  }

  int intersection(int u, int v, int a, int b) {
    std::vector<int> t = {lca(u, a), lca(u, b), lca(v, a), lca(v, b)};
    std::sort(t.begin(), t.end());
    int r = lca(u, v), R = lca(a, b);
    if (dep[t[0]] < std::min(dep[r], dep[R]) || dep[t[2]] < std::max(dep[r], dep[R])) {
      return 0;
    }
    return 1 + clac(t[2], t[3]);
  }

  std::unordered_map<int, std::vector<pii>> virtual_tree(std::vector<int> &k) {
    std::unordered_map<int, std::vector<pii>> g;
    auto cmp = [&](int x, int y) {
      return dfn[x] < dfn[y];
    };
    std::sort(k.begin(), k.end(), cmp);
    std::vector<int> a;
    if (k[0] != 1)
      a.push_back(1);
    for (int i = 0; i < k.size() - 1; i++) {
      a.push_back(k[i]);
      a.push_back(lca(k[i], k[i + 1]));
    }
    a.push_back(k.back());
    std::sort(a.begin(), a.end(), cmp);
    int len = std::distance(a.begin(), std::unique(a.begin(), a.end()));
    for (int i = 0; i < len - 1; i++) {
      int lc = lca(a[i], a[i + 1]);
      g[lc].emplace_back(a[i + 1], query(lc, a[i + 1]));
      // 有边权的就连dis(lc, a[i + 1])
    }
    return g;
  }
};
