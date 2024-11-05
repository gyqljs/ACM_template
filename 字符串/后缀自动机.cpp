#include <bits/stdc++.h>

struct sam {
  struct node {
    int len, link;
    std::array<int, 26> son;
    int &operator[](int id) { return son[id]; }
  };

  std::vector<node> t;

  explicit sam(): t(2) { t[0].son.fill(1), t[0].len = -1; }

  // 返回添加新字符的节点编号
  // int p = 1, p = extend(p, c - 'a');
  int extend(int p, int c) {
    if (t[p][c]) {
      int q = t[p][c];
      if (t[q].len == t[p].len + 1)
        return q;
      int r = t.size();
      t.emplace_back();
      t[r].len = t[p].len + 1;
      t[r].link = t[q].link;
      t[r].son = t[q].son;
      t[q].link = r;
      while (t[p][c] == q) {
        t[p][c] = r;
        p = t[p].link;
      }
      return r;
    }
    int cur = t.size();
    t.emplace_back();
    t[cur].len = t[p].len + 1;
    while (!t[p][c]) {
      t[p][c] = cur;
      p = t[p].link;
    }
    t[cur].link = extend(p, c);
    return cur;
  }
  node operator[](int id) { return t[id]; }
};

int64_t ans = 0;


int cnt[2000006];
sam sm;

std::unordered_map<int, std::vector<int>> mp;

void dfs(int u) {
  for (auto v: mp[u]) {
    dfs(v);
    cnt[u] += cnt[v];
  }
  if (cnt[u] > 1)
    ans = std::max(ans, (int64_t) cnt[u] * sm[u].len);
}


int32_t main() {
  std::string s;
  std::cin >> s;
  int p = 1;
  for (char c: s) {
    cnt[p = sm.extend(p, c - 'a')]++;
  }
  for (int i = 2; i < sm.t.size(); i++)
    mp[sm[i].link].push_back(i);
  dfs(1);
  std::cout << ans << '\n';
}
