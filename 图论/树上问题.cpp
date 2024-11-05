#include <bits/stdc++.h>

struct tree {
  int n;
  std::vector<std::vector<std::pair<int, int>>> e;
  std::vector<int> dep, f, md, d1, d2, s1, s2, up;

  explicit tree(int n): n(n - 1), e(n), dep(n), f(n), md(n), d1(n), d2(n), s1(n), s2(n), up(n) {}

  void add(int u, int v, int w) {
    e[u].emplace_back(w, v);
    e[v].emplace_back(w, u);
  }

  void dfs(int u, int fa) {
    md[u] = dep[u];
    for (auto [w, v]: e[u]) {
      if (v == fa)
        continue;
      dep[v] = dep[u] + 1;
      f[v] = u;
      dfs(v, u);
      md[u] = std::max(md[u], md[v]);
    }
  }

  void dfs1(int u, int fa) {
    for (auto [w, v]: e[u]) {
      if (v == fa) continue;
      dfs1(v, u);
      int x = d1[v] + w;
      if (x > d1[u]) {
        d2[u] = d1[u], s2[u] = s1[u];
        d1[u] = x, s1[u] = v;
      } else if (x > d2[u]) {
        d2[u] = x, s2[u] = v;
      }
    }
  }

  void dfs2(int u, int fa) {
    for (auto [w, v]: e[u]) {
      if (v == fa) continue;
      if (s1[u] == v) {
        up[v] = std::max(up[u], d2[u]) + w;
      } else {
        up[v] = std::max(up[u], d1[u]) + w;
      }
      dfs2(v, u);
    }
  }

  void work(int root) {
    dfs(root, 0), dfs1(root, 0), dfs2(root, 0);
  }

  // dia 直径 cnt 中心 cog重心
  int rad{}, cnt{}, dia{};

  void getCenter() {
    cnt = 1; // 中心
    for (int i = 1; i <= n; i++) {
      if (std::max(d1[i], up[i]) < std::max(d1[cnt], up[cnt])) {
        cnt = i;
      }
    }
    rad = std::max(d1[cnt], up[cnt]); // 距离最远点的距离的最小值
    dia = d1[cnt] + up[cnt] + 1; // 直径
  }

  int rem = INT_MAX; // 删除重心后剩余连通块体积的最小值
  int cog = 1; // 重心
  std::vector<bool> vis;

  void getCog() {
    vis.resize(n);
    dfsCog(1);
  }

  int dfsCog(int u) {
    vis[u] = true;
    int s = 1, res = 0;
    for (auto [w, v]: e[u]) {
      if (vis[v]) continue;
      int t = dfsCog(v);
      res = std::max(res, t);
      s += t;
    }
    res = std::max(res, n - s);
    if (res < rem) {
      rem = res;
      cog = u;
    }
    return s;
  }
};

class dsu_on_tree {
public:
  // col[x]:节点的颜色编号,  cnt[i]:颜色编号i的数量
  std::vector<int> col, sz, son, cnt, ans;
  std::vector<std::vector<int>> e;

  void dfs1(int u, int fa) { // 重链剖分
    sz[u] = 1;
    for (int v: e[u]) {
      if (v == fa)
        continue;
      dfs1(v, u);
      sz[u] += sz[v];
      if (sz[v] > sz[son[u]])
        son[u] = v;
    }
  }

  int64_t sum = 0, mx = 0;
  void add(int u, int fa, int son);

  void sub(int u, int fa) {
    cnt[col[u]]--;
    for (int v: e[u]) {
      if (v != fa)
        sub(v, u);
    }
  }

  void dfs2(int u, int fa, bool opt) {
    for (int v: e[u]) {
      if (v != fa && v != son[u])
        dfs2(v, u, 0);
    }
    if (son[u]) dfs2(son[u], u, 1); // 后搜重儿子

    add(u, fa, son[u]); // 累加x和轻子树贡献
    ans[u] = sum; // 存储答案
    if (!opt) sub(u, fa), sum = mx = 0; // 减掉轻子树贡献
  }

  dsu_on_tree(int n): col(n), sz(n), son(n), cnt(n), ans(n), e(n) {}

  auto &operator[](int id) { return e[id]; }

  void work() {
    dfs1(1, 0);
    dfs2(1, 0, 0);
  }
};

void dsu_on_tree::add(int u, int fa, int son) {
  cnt[col[u]]++;
  if (cnt[col[u]] > mx)
    mx = cnt[col[u]], sum = col[u];
  else if (cnt[col[u]] == mx)
    sum += col[u];

  for (int v: e[u]) {
    if (v != fa && v != son)
      add(v, u, son);
  }
}
