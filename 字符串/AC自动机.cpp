#include <bits/stdc++.h>

class ac_maton {
public:
  int n = 0;
  struct node {
    // 失配指针,标记是否为模式串的结尾,统计匹配次数,子节点，26个字母 + 1个其他标记
    int fail = 0, len = 0, ans = 0;
    std::array<int, 26> son;
    int &operator[](int v) { return son[v]; }
  };
  std::queue<int> q;
  std::vector<node> t;
  std::vector<int> rev, in;

  explicit ac_maton(int sz = 2): t(2), rev(1) {}

  void insert(std::string &s) {
    n++;
    int u = 1;
    for (char i: s) {
      int v = i - 'a';
      if (!t[u][v]) {
        t[u][v] = t.size();
        t.emplace_back(node());
      }
      u = t[u][v];
    }
    if (!t[u].len)
      t[u].len = n;
    rev.push_back(t[u].len); // 记录模式串 num 对应的字典树节点标记
  }

  // 预处理失配指针
  void work() {
    in.resize(t.size());
    for (int i = 0; i < 26; i++)
      t[0][i] = 1;
    q.push(1);
    t[1].fail = 0;
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      int Fail = t[u].fail;
      for (int i = 0; i < 26; i++) {
        int v = t[u][i];
        if (!v) {
          t[u][i] = t[Fail][i];
          continue;
        }
        t[v].fail = t[Fail][i];
        in[t[Fail][i]]++; // 更新失配指针对应节点的入度
        q.push(v);
      }
    }
  }

  std::vector<int> query(std::string &s) {
    int u = 1;
    for (char i: s)
      u = t[u][i - 'a'], t[u].ans++;
    std::vector<int> vis(n + 1);
    for (int i = 1; i < t.size(); i++)
      if (!in[i]) q.push(i);
    while (!q.empty()) {
      int fr = q.front();
      q.pop();
      vis[t[fr].len] = t[fr].ans; // 记录模式串匹配次数
      int u = t[fr].fail;
      t[u].ans += t[fr].ans;
      if (!(--in[u]))
        q.push(u); // 更新入度，入度为 0 的节点入队
    }
    std::vector<int> ans(n + 1);
    for (int i = 1; i <= n; i++)
      ans[i] = vis[rev[i]];
    return ans;
  }

  node &operator[](int id) { return t[id]; }
};