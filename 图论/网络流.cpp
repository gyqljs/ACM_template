#include <bits/stdc++.h>

// 全局最小割 = 最大流
// 最大权闭合子图 = 全部正权点的和 - 最小割
// 二分图最小顶点集覆盖数 = 二分图最大匹配数。
// 对于全局最小割点 把每个点拆成两个 一个连入边 另外一个连出边

// 存图方式 链式前向星

const int inf = 2e9;

class dinic_netflow {
public:
  int n;
  struct edge {
    int t = 0, n = 0, w = 1;
    explicit edge(int a, int b, int c = 1): t(a), n(b), w(c) {}
  };

  std::vector<edge> e;
  std::vector<int> h, d, cur;

  void add_edge(int u, int v, int wi) {
    e.emplace_back(v, h[u], wi);
    h[u] = e.size() - 1;
  }

  void add(int u, int v, int w = 1) {
    add_edge(u, v, w);
    add_edge(v, u, 0);
  }
  explicit dinic_netflow(int n): n(n), h(n, -1), d(n), cur(n) { e.reserve(114154); }


  // add(u, v, w), add(v, u, 0);

  bool bfs(int s, int t) {
    std::fill(d.begin(), d.end(), 0);

    std::queue<int> q;
    q.push(s);
    d[s] = 1;

    while (!q.empty()) {
      int u = q.front();
      q.pop();

      for (int i = h[u]; i != -1; i = e[i].n) {
        int v = e[i].t;
        if (d[v] == 0 && e[i].w) {
          d[v] = d[u] + 1;
          q.push(v);
          if (v == t)
            return true;
        }
      }
    }
    return false;
  }

  int64_t dfs(int u, int t, int mf) {
    if (u == t)
      return mf;
    int64_t sum = 0;
    for (int i = cur[u]; i != -1; i = e[i].n) {
      cur[u] = i; // 当前弧优化
      int v = e[i].t;
      if (d[v] == d[u] + 1 && e[i].w) {
        int64_t f = dfs(v, t, std::min(mf, e[i].w));
        if (!f)
          continue;
        e[i].w -= f;
        e[i ^ 1].w += f; // 更新残留网
        sum += f; // 累加u的流出流量
        mf -= f; // 减少u的剩余流量
        if (mf == 0)
          break; // 余量优化
      }
    }
    if (sum == 0)
      d[u] = 0; // 残枝优化
    return sum;
  }

  int64_t dinic(int s, int t) {
    int64_t flow = 0;

    while (bfs(s, t)) {
      cur.assign(h.begin(), h.end());
      flow += dfs(s, t, inf);
    }

    return flow;
  }

  void print(int now, int s, int t);

  void print(int s, int t);
};

void dinic_netflow::print(int now, int s, int t) {
  std::cout << now << ' ';
  for (int i = h[now]; i != -1; i = e[i].n) {
    if (e[i].w || e[i].t == s || e[i].t == now + n) continue;
    print(e[i].t - n, s, t);
  }
}


void dinic_netflow::print(int s, int t) {
  // 如果是二分图打印全部的可行路径
  for (int i = 1; i <= n; ++i) {
    bool f = false;
    for (int j = h[i + n]; j != -1; j = e[j].n) // 查找yi
      if (e[j].w && e[j].t != i && e[j].t != t) {
        f = true;
        break;
      }
    if (!f) {
      print(i, s, t);
      std::cout << '\n';
    }
  }

  // 如果是二分图 输出一个可行的配对方案
  for (int i = 0; i < e.size(); i += 2) {
    if (e[i].t != s and e[i ^ 1].t != t and e[i].t != t and e[i ^ 1].t != s) {
      if (e[i ^ 1].w != 0)
        std::cout << e[i ^ 1].t << ' ' << e[i].t << '\n';
    }
  }
}


// 基础模型：构筑二分图，左半部n个点代表盈利项目，右半部m个点代表材料成本收益为盈利之和减去成本之和，求最大收益?
// 建图：建立源点S向左半部连边，建立汇点T向右半部连边，如果某个项目需要某个材料，则新增一条容量inf的跨部边。
// 割边：放弃某个项目则断开S至该项目的边，购买某个原料则断开该原料至T的边，最终的图一定不存在从S到T
// 的路径，此时我们得到二分图的一个S - T割。此时最小割即为求解最大流，边权之和减去最大流即为最大收益。



class cost_netflow {
public:
  struct edge {
    int t, n, w, c;
    edge(int t, int n, int w, int c): t(t), n(n), w(w), c(c) {}
  };
  int n;
  std::vector<edge> e;
  std::vector<int> h, pre;
  std::vector<int64_t> cur, d;

  explicit cost_netflow(int n): n(n), h(n + 1, -1) {}

  void add_edge(int u, int v, int w, int c) {
    e.emplace_back(v, h[u], w, c);
    h[u] = e.size() - 1;
  }

  void add(int u, int v, int w, int c) {
    add_edge(u, v, w, c);
    add_edge(v, u, 0, -c);
  }

  bool dijkstra(int s, int t) {
    d.assign(n + 1, INT64_MAX);
    pre.assign(n + 1, 0);
    std::priority_queue<std::pair<int64_t, int>> q;
    d[s] = 0;
    q.emplace(0, s);
    while (!q.empty()) {
      auto [dis, u] = q.top();
      dis = -dis;
      q.pop();
      if (d[u] < dis)
        continue;
      for (int i = h[u]; i != -1; i = e[i].n) {
        int v = e[i].t;
        if (e[i].w && d[v] > dis + cur[u] - cur[v] + e[i].c) {
          d[v] = dis + cur[u] - cur[v] + e[i].c;
          pre[v] = i;
          q.emplace(-d[v], v);
        }
      }
    }
    return d[t] != INT64_MAX;
  }

  std::pair<int64_t, int64_t> flow(int s, int t) {
    int64_t flow = 0, cost = 0;
    cur.assign(n, 0);
    while (dijkstra(s, t)) {
      for (int i = 0; i < n; ++i)
        cur[i] += d[i];
      int aug = inf;
      for (int i = t; i != s; i = e[pre[i] ^ 1].t)
        aug = std::min(aug, e[pre[i]].w);
      for (int i = t; i != s; i = e[pre[i] ^ 1].t) {
        e[pre[i]].w -= aug;
        e[pre[i] ^ 1].w += aug;
      }
      flow += aug;
      cost += int64_t(aug) * cur[t];
    }
    return {flow, cost};
  }
};

class hlpp_netflow {
public:
  int n, s, t;

  struct edge {
    int n, t;
    long long w;
  };

  std::vector<edge> e;
  std::vector<int> h, d, gap;
  std::vector<int64_t> ex;
  std::vector<std::vector<int>> sk;
  int level = 0;

  hlpp_netflow(int n): n(n), h(n, -1), d(n + 1, inf), ex(n), gap(n + 1), sk(n + 1) {}

  void add_edge(int u, int v, int wi) {
    e.emplace_back(edge{h[u], v, wi});
    h[u] = e.size() - 1;
  }

  void add(int u, int v, int w = 1) {
    add_edge(u, v, w);
    add_edge(v, u, 0);
  }

  // 从节点 u 推送流量
  int push(int u) {
    bool init = (u == s); // 检查u是否为源节点
    for (int i = h[u]; i != -1; i = e[i].n) {
      int v = e[i].t;
      int64_t w = e[i].w;

      // 如果不满足容量或高度条件则跳过
      if (w == 0 || (!init && d[u] != d[v] + 1) || d[v] == inf)
        continue;
      // 确定推动量
      int64_t k = init ? w : std::min(w, ex[u]);
      // 如果推送到变为活动的节点，请将其添加到存储桶中
      if (v != s && v != t && ex[v] == 0) {
        if (d[v] < sk.size()) { // 添加检查以防止越界
          sk[d[v]].push_back(v);
          level = std::max(level, d[v]);
        }
      }
      // 执行推送
      ex[u] -= k;
      ex[v] += k;
      e[i].w -= k;
      e[i ^ 1].w += k;
      // 如果没有剩余，则返回
      if (ex[u] == 0)
        return 0;
    }
    return 1; // 表示可能需要重新标记
  }

  // 通过更新节点 u 的高度来重新标记节点 u
  void relabel(int u) {
    d[u] = inf;
    for (int i = h[u]; i != -1; i = e[i].n) {
      if (e[i].w > 0)
        d[u] = std::min(d[u], d[e[i].t]);
    }
    if (d[u] < inf)
      d[u]++;
    else
      d[u] = n + 1; // 处理无法重新标记的情况

    if (d[u] < n) {
      sk[d[u]].push_back(u);
      level = std::max(level, d[u]);
      gap[d[u]]++;
    }
  }

  // 使用接收器的反向 BFS 初始化高度
  bool bfs() {
    fill(d.begin(), d.end(), inf);
    std::queue<int> q;
    q.push(t);
    d[t] = 0;

    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int i = h[u]; i != -1; i = e[i].n) {
        int v = e[i].t;
        if (e[i ^ 1].w > 0 && d[v] > d[u] + 1) {
          d[v] = d[u] + 1;
          q.push(v);
        }
      }
    }
    return d[s] != inf;
  }

  // 选择具有最高标签的下一个活动节点
  int select_node() {
    while (level >= 0 && sk[level].empty())
      level--;
    if (level == -1)
      return 0;
    // 获取当前最高桶中最后添加的节点
    int u = sk[level].back();
    sk[level].pop_back();
    return u;
  }

  // 执行 HLPP 算法并返回最大流
  int64_t hlpp(int s0, int t0) {
    s = s0, t = t0;

    if (!bfs())
      return 0;

    // 初始化间隙数组
    for (int i = 0; i < n; i++) {
      if (d[i] != inf)
        gap[d[i]]++;
    }

    // 从源头初始化预流
    d[s] = n;
    for (int i = h[s]; i != -1; i = e[i].n) {
      int v = e[i].t;
      int64_t w = e[i].w;
      if (v != s && v != t && w > 0) {
        ex[s] -= w;
        ex[v] += w;
        e[i].w = 0;
        e[i ^ 1].w += w;
        if (ex[v] > 0) {
          if (d[v] < sk.size()) { // 添加检查以防止越界
            sk[d[v]].push_back(v);
            level = std::max(level, d[v]);
          }
        }
      }
    }

    int u;
    while ((u = select_node())) {
      if (push(u)) { // 如果推送不成功，则重新标记
        if (--gap[d[u]] == 0) {
          // 间隙启发式：所有高度 > d[u] 的节点都是不可达的
          for (int i = 0; i < n; i++) {
            if (i != s && d[i] > d[u] && d[i] < inf)
              d[i] = inf;
          }
        }
        relabel(u);
      }
    }

    return ex[t];
  }
};


class upper_lower_netflow {
public:
  dinic_netflow f;
  int sup_s, sup_t;
  std::vector<int> w;

  explicit upper_lower_netflow(int n): f(n + 2), sup_s(n), sup_t(n + 1), w(n + 2) {}

  void add(int u, int v, int l, int r) {
    if (l > r)
      return;
    w[u] -= l;
    w[v] += l;
    f.add(u, v, r - l);
  }

  bool check(int s, int t) {
    for (int u = 0; u < f.h.size() - 2; ++u) {
      if (w[u] > 0) {
        f.add(sup_s, u, w[u]);
      } else if (w[u] < 0) {
        f.add(u, sup_t, -w[u]);
      }
    }
    f.add(t, s, inf);

    int64_t sum = 0;
    for (int u = 0; u < f.h.size() - 2; ++u) {
      if (w[u] > 0) {
        sum += w[u];
      }
    }
    // 无源汇最大流
    int64_t flow = f.dinic(sup_s, sup_t);
    if (flow < sum) {
      return false;
    }

    return true;
  }

  int64_t max_flow(int s, int t) {
    if (check(s, t))
      return f.dinic(s, t);
    else
      return -1;
  }
};
