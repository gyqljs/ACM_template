#include <bits/stdc++.h>

std::vector<int> sz;
int n, m;
std::vector<std::vector<std::pair<int, int>>> e;
void dfs(int u, int fa) {
  sz[u] = 1;
  for (auto [v, w]: e[u]) {
    if (v == fa) continue;
    dfs(v, u);
    for (int j = sz[u]; ~j; --j) {

      // if (m < j) continue;
      // 枚举子节点v
      for (int k = std::min(m - j, sz[v]); ~k; --k) {
        if (m < j + k)
          continue;
        // 写你的dp方程
        // dp[u][j + k] = f(dp[u][j] + dp[v][k]);
      }
    }
    sz[u] += sz[v];
  }
}
