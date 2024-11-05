#include <bits/stdc++.h>

const int N = 5001;
// 图的邻接矩阵
bool rela[N][N], inb[N];
class sat {
public:
  int n, tim = 0, cnt = 0;
  std::vector<std::vector<int>> e;
  std::vector<int> dfn, low, scc, vis, ans;

  std::stack<int> stk;

  void tarjan(int x) {
    dfn[x] = low[x] = ++tim;
    stk.push(x);
    for (int y: e[x]) {
      if (!dfn[y]) {
        tarjan(y);
        low[x] = std::min(low[x], low[y]);
      } else if (!scc[y])
        low[x] = std::min(low[x], dfn[y]);
    }

    if (low[x] == dfn[x]) {
      ++cnt;
      int y;
      do {
        y = stk.top();
        stk.pop();
        scc[y] = cnt;
      } while (y != x);
    }
  }

  void dfs(int x) {
    vis[x] = 1;
    for (int y: e[x])
      if (!vis[y])
        dfs(y);
  }

  bool check(int x) {
    std::fill(vis.begin(), vis.end(), 0);
    dfs(x);
    if (vis[x] and vis[x < n ? x + n : x - n])
      return 1;
    return 0;
  }


  explicit sat(int n): n(n), e(2 * n), dfn(2 * n), low(2 * n), scc(2 * n), vis(2 * n), ans(2 * n) {}

  void add(int i, bool a, int j, bool b) {
    e[i + !a * n].push_back(j + b * n);
    e[j + !b * n].push_back(i + a * n);
  }

  bool work() {
    for (int i = 0; i < 2 * n; i++) {
      if (!dfn[i])
        tarjan(i);
    }

    for (int i = 0; i < n; i++) {
      if (scc[i] == scc[i + n]) {
        return false;
      }
    }


    return true;
  }

  void col1() {
    for (int i = 0; i < n; i++) {
      ans[i] = scc[i] > scc[i + n];
    }
  }

  void col2() {
    for (int i = 0; i < n; i++) {
      int x = check(i), y = check(i + n);
      if (x && !y)
        ans[i] = 1;
      else if (!x && y)
        ans[i] = 0;
      else
        ans[i] = 1314;
    }
  }



  // 统计多少符合要求的解 需要同时初始化邻接矩阵
  int col3() {
    std::vector<int> a, b, num(n), id(n);
    for (int i = 0; i < n; i++) {
      if (scc[i] < scc[i + n])
        a.push_back(i);
      else
        b.push_back(i), inb[i] = 1;
    }

    for (int i: a) {
      for (int j: b) {
        if (rela[i][j]) {
          num[i]++;
          id[i] = j;
        }
      }
    }

    for (int i: b) {
      for (int j: a) {
        if (!rela[i][j]) {
          num[i]++;
          id[i] = j;
        }
      }
    }

    int ans = 0;
    for (int i = 0; i < n; i++) {
      if (num[i] == 1 and !num[id[i]])
        ans++;
      if (!num[i] and ((inb[i] and b.size() > 1) or (!inb[i] and a.size() > 1)))
        ans++;
    }
    if (a.size() and b.size())
      ans++;
    return ans;
  }

  auto &operator[](int id) { return e[id]; }
};
