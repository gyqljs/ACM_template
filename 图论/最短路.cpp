#include <bits/stdc++.h>

struct edge {
  int t, w;
  explicit edge(int a = 0, int b = 1): t(a), w(b) {}
  bool operator<(const edge &oth) const { return w < oth.w; }
};

std::vector<std::vector<edge>> g;


void dijkstra(int st, std::vector<int> &ans) {
  std::vector<bool> b(g.size());
  ans.resize(g.size(), INT32_MAX);
  ans[st] = 0;
  // 维护边长的队列
  std::priority_queue<edge> pq;
  pq.emplace(st, 0);
  while (!pq.empty()) {
    auto [v, _] = pq.top();
    pq.pop();
    if (!b[v]) {
      b[v] = true;
      for (auto j: g[v]) {
        // 松弛操作
        if (ans[j.t] > ans[v] + j.w) {
          ans[j.t] = ans[v] + j.w;
          pq.emplace(j.t, ans[j.t]);
        }
      }
    }
  }
}

// 负环返回ture 差分约束就是使用spfa找最长路
bool spfa(int st, std::vector<int> &ans) {
  std::queue<int> q;
  std::vector<bool> b(g.size());
  ans.resize(g.size(), INT32_MAX);
  ans[st] = 0;
  q.push(st);
  std::vector<int> check(g.size(), 0);
  b[st] = true;
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    b[u] = false;
    for (auto j: g[u]) {
      if (ans[j.t] > ans[u] + j.w) {
        ans[j.t] = ans[u] + j.w;
        check[j.t] = check[u] + 1;
        if (check[j.t] >= g.size()) {
          return true;
        }
        if (b[j.t] == 0) {
          q.push(j.t);
          b[j.t] = true;
        }
      }
    }
  }
  return false;
}

// 差分约束
// add_edge(0, i, 0) add_edge(0, i, 0)
// if (g.spfa(0, ans)) std::cout << "NO" << '\n';
// else 输出ans

// 注意 这个函数会改变图的边权！！！！
void johnson(std::vector<std::vector<int>> &ans) {
  ans.assign(g.size(), std::vector<int>(g.size() + 1, 0));
  for (int i = 1; i < g.size(); i++)
    g[0].emplace_back(0, i);
  std::vector<int> h, d;
  for (int i = 1; i < g.size(); i++) {
    for (auto j: g[i]) {
      j.w += h[i] - h[j.t];
    }
  }
  for (int i = 1; i < g.size(); i++) {
    dijkstra(i, d);
    for (int j = 1; j < g.size(); j++) {
      if (d[j] == INT32_MAX)
        ans[i][j] += INT32_MAX;
      else
        ans[i][j] += (d[j] + h[j] - h[i]);
    }
    d.clear();
  }
}