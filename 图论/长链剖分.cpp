#include <bits/stdc++.h>


struct lld {
  int n, *p;
  std::vector<int> len, son;
  std::vector<int *> f;

  std::vector<std::vector<int>> e;


  explicit lld(int n): n(n), len(n), son(n), f(n), e(n) { p = new int[n]; }

  void work(int u, int fa) {
    for (int v: e[u]) {
      if (v == fa)
        continue;
      work(v, u);
      if (len[v] >= len[son[u]])
        son[u] = v;
    }
    len[u] = len[son[u]] + 1;
  }


  void dfs(int u, int fa) {
    f[u][0] = 1;
    if (son[u]) {
      // 继承长儿子答案
      f[son[u]] = f[u] + 1;
      dfs(son[u], u);
    }
    for (int v: e[u]) {
      if (v == fa or v == son[u])
        continue;
      f[v] = p, p += len[v];
      dfs(v, u);
      for (int j = 1; j <= len[v]; j++) {
        f[u][j] += f[v][j - 1]; // 暴力转移短儿子的信息
      }
    }
  }


  void solve() {
    work(1, 0);
    f[1] = p;
    p += len[1];
    dfs(1, 0);
  }


  void add(int u, int v) { e[u].push_back(v); }

  auto operator[](int id) { return f[id]; }
};

// 两个数组怎么维护?
struct dlld {
  int n, *p;
  std::vector<int> len, son;
  std::vector<int *> f, g;

  std::vector<std::vector<int>> e;


  explicit dlld(int n): n(n), len(n), son(n), f(n), g(n), e(n) { p = new int[n]; }

  void work(int u, int fa) {
    for (int v: e[u]) {
      if (v == fa)
        continue;
      work(v, u);
      if (len[v] >= len[son[u]])
        son[u] = v;
    }
    len[u] = len[son[u]] + 1;
  }


  void dfs(int u, int fa) {
    f[u][0] = 1;
    if (son[u]) {
      // 继承长儿子答案
      f[son[u]] = f[u] + 1;
      g[son[u]] = g[u] - 1;
      dfs(son[u], u);
    }
    for (int v: e[u]) {
      if (v == fa or v == son[u])
        continue;
      f[v] = p, p += len[v] << 1;
      g[v] = p, p += len[v] << 1;
      dfs(v, u);
      for (int j = 1; j <= len[v]; j++) { // 暴力合并轻儿子的信息
        g[u][j] += f[u][j] * f[v][j - 1];
      }
      for (int j = 1; j <= len[v]; j++)
        f[u][j] += f[v][j - 1], g[u][j - 1] += g[v][j];
    }
  }


  void solve() {
    work(1, 0);
    f[1] = p;
    p += len[1];
    dfs(1, 0);
  }


  void add(int u, int v) { e[u].push_back(v); }

  auto operator[](int id) { return f[id]; }
};
