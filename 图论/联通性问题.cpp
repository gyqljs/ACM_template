#include <bits/stdc++.h>

// 存图方式 链式邻接表
class graph {
public:
  struct edge {
    int u, v, w;
    bool operator<(const edge &oth) const {
      return u == oth.u ? v < oth.v : u < oth.u;
    }
  };
  int n;
  std::vector<int> w;
  std::vector<edge> e;
  std::vector<std::vector<int>> h;

  // 构造函数
  explicit graph(int n): n(n), w(n + 1), h(n + 1) {}

  // 加入边
  void add_edge(int u, int v, int w = 1) {
    h[u].push_back(e.size());
    e.push_back({u, v, w});
  }

  int &operator[](int id) { return w[id]; }

  graph tarjan_scc() {
    std::vector<bool> b(n + 1, 0);
    // dfn 节点i的时间戳 表示从根开始什么时候可以访问到
    std::vector<int> dfn(n + 1), low(n + 1), scc(n + 1);
    std::stack<int> tk;
    int cnt = 0, y;
    //  cnt记录强联通分量的数量
    std::function<void(int)> dfs = [&](int x) {
      // 节点本身是一个强联通分量
      dfn[x] = low[x] = tk.size() + 1;
      tk.push(x);
      // 节点入栈
      b[x] = true;
      for (int j: h[x]) {
        if (!dfn[e[j].v]) {
          dfs(e[j].v);
          low[x] = std::min(low[x], low[e[j].v]);
        } else if (b[e[j].v]) {
          low[x] = std::min(low[x], low[e[j].v]);
        }
      }
      if (dfn[x] == low[x]) {
        cnt++;
        do {
          y = tk.top();
          tk.pop();
          scc[y] = cnt;
          b[y] = false;
        } while (y != x);
      }
    };
    for (int i = 1; i <= n; i++) {
      if (!dfn[i])
        dfs(i);
    }
    // 重构图
    graph g(cnt);
    for (int i = 1; i <= n; i++) {
      g.w[scc[i]] += w[i];
      for (int j: h[i]) {
        if (scc[i] != scc[e[j].v])
          g.add_edge(scc[i], scc[e[j].v], e[j].w);
      }
    }
    return g;
  }

  // 加边的时候特判自环
  std::set<int> vdcc(std::vector<std::vector<int>> &ans) {
    // dfn 节点i的时间戳 表示从根开始什么时候可以访问到
    std::vector<int> dfn(n + 1), low(n + 1);
    std::stack<int> tk;
    int root;
    // 割点的编号
    std::set<int> poi;
    int cnt = 0;
    std::function<void(int, int)> dfs = [&](int x, int c_edge) {
      // 节点本身是一个强联通分量
      dfn[x] = low[x] = (int) tk.size() + 1;
      tk.push(x);
      if (h[x].size() == 0)
        ans.push_back({x});
      for (int i: h[x]) {
        int v = e[i].v;
        if (!dfn[v]) {
          dfs(v, x);
          low[x] = std::min(low[x], low[v]);
          if (low[v] >= dfn[x]) {
            ans.push_back({});
            int t;
            do { // 出栈操作
              t = tk.top();
              ans.back().push_back(t);
              tk.pop();
            } while (t != v);
            ans.back().push_back(x);
          }
        } else
          low[x] = std::min(low[x], dfn[v]);
      }
    };

    for (root = 1; root <= n; root++)
      if (!dfn[root])
        dfs(root, 0);
    return poi;
  }

  std::set<edge> edcc(std::vector<std::vector<int>> &ans) {
    // dfn 节点i的时间戳 表示从根开始什么时候可以访问到
    std::vector<int> dfn(n + 1), low(n + 1);
    std::stack<int> tk;
    std::set<edge> ege;
    int root;
    std::vector<bool> b(n + 1);
    std::function<void(int, int)> dfs = [&](int x, int c_edge) {
      // 节点本身是一个强联通分量
      dfn[x] = low[x] = (int) tk.size() + 1;
      int child = 0;
      tk.push(x);
      b[x] = true;
      // 节点入栈
      for (int j: h[x]) {
        if (!dfn[e[j].v]) {
          dfs(e[j].v, j);
          low[x] = std::min(low[x], low[e[j].v]);
          // 找割边
          if (low[e[j].v] > dfn[x])
            ege.insert(e[j]);
        } else if (j != (c_edge ^ 1))
          low[x] = std::min(low[x], dfn[e[j].v]);
      }
      int cnt = 0;
      if (dfn[x] == low[x]) {
        ans.push_back({});
        int t;
        do {
          t = tk.top();
          ans.back().push_back(t);
          tk.pop();
        } while (t != x);
      }
    };
    for (root = 1; root <= n; root++) {
      if (!dfn[root])
        dfs(root, 0);
    }
    return ege;
  }
};