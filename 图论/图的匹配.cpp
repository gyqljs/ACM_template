#include <bits/stdc++.h>

// 二分图最大权完美匹配
struct km {
  std::vector<int> ans;

  int64_t res = 0;

  explicit km(std::vector<std::vector<int>> &map): ans(map.size(), -1) {
    int n = map.size() - 1;
    std::vector<int64_t> ex(map.size()), ey(map.size()); // ex,ey顶标
    std::vector<bool> vy(map.size());
    for (int i = 1; i <= n; i++) {
      vy.assign(vy.size(), false);
      int64_t x, y = 0, dy = 0, delta;
      std::vector<int64_t> pre(n + 1), s(n + 1, 1e18);
      ans[y] = i;
      while (1) {
        x = ans[y];
        delta = 1e18;
        vy[y] = 1;
        for (int i = 1; i <= n; i++) {
          if (vy[i]) continue;
          if (s[i] > ex[x] + ey[i] - map[x][i]) {
            s[i] = ex[x] + ey[i] - map[x][i];
            pre[i] = y;
          }
          if (s[i] < delta) {
            delta = s[i];
            dy = i;
          }
        }
        for (int i = 0; i <= n; i++) {
          if (vy[i])
            ex[ans[i]] -= delta, ey[i] += delta;
          else
            s[i] -= delta;
        }
        y = dy;
        if (ans[y] == -1)
          break;
      }
      while (y) {
        ans[y] = ans[pre[y]];
        y = pre[y];
      }
    }
    for (int i = 1; i <= n; i++)
      if (ans[i] != -1)
        res += map[ans[i]][i];
  }
};
