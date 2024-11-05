#include <bits/stdc++.h>



template<class T>
struct hld {
  int n, cnt = 0;
  //
  std::vector<int> son, top, id, fa, dep, sz, nw, w, bot;
  std::vector<std::vector<int>> e;
  hld(int n): n(n), e(n), son(n), top(n), id(n), fa(n), dep(n), sz(n), nw(n), w(n), bot(n) {}

  T a;

  void add(int u, int v) {
    e[u].push_back(v);
  }

  int &operator[](int id) { return w[id]; }

  void dfs1(int u, int f) {
    fa[u] = f, dep[u] = dep[f] + 1, sz[u] = 1;
    for (int j: e[u]) {
      if (j != f) {
        dfs1(j, u);
        sz[u] += sz[j];
        if (sz[son[u]] < sz[j])
          son[u] = j;
      }
    }
  }


  void dfs2(int u, int t) {
    top[u] = t, id[u] = ++cnt, nw[cnt] = w[u];
    if (son[u]) {
      dfs2(son[u], t);
      for (int j: e[u]) {
        if (j != fa[u] and j != son[u])
          dfs2(j, j);
      }
    }
  }

  int lca(int x, int y) {
    while (top[x] != top[y]) {
      if (dep[top[x]] > dep[top[y]]) {
        x = fa[top[x]];
      } else {
        y = fa[top[y]];
      }
    }
    return dep[x] < dep[y] ? x : y;
  }


  int clac(int x, int y) { // 查询两点间距离
    return dep[x] + dep[y] - 2 * dep[lca(x, y)];
  }

  void work(int root = 1) {
    dfs1(root, 0);
    dfs2(root, root);
    a = a(nw);
  }

  int query(int x) {
    return a.query(id[x], id[x] + sz[x] - 1);
  }

  int query(int u, int v) {
    int ans = 0;
    while (top[u] != top[v]) {
      if (dep[top[u]] < dep[top[v]])
        std::swap(u, v);
      ans += a.query(id[top[u]], id[u]);
      u = fa[top[u]];
    }
    if (dep[u] < dep[v])
      std::swap(u, v);
    ans += a.query(id[v], id[u]);
    return ans;
  }

  void change(int x, int v) {
    a.change(id[x], id[x] + sz[x] - 1, v);
  }

  // 修改u到v所有的点权
  void change(int u, int v, int w) {
    while (top[u] != top[v]) {
      if (dep[top[u]] < dep[top[v]])
        std::swap(u, v);
      a.change(id[top[u]], id[u], w);
      u = fa[top[u]];
    }
    if (dep[u] < dep[v])
      std::swap(u, v);
    a.change(id[v], id[u], w); // 最后一段
  }
};