#include <bits/stdc++.h>

struct pam {
  struct node {
    int len, link, cnt;
    std::array<int, 26> son;
    int &operator[](int id) { return son[id]; }
  };
  std::vector<node> t;
  std::string s;
  int suf;
  explicit pam(): t(2), suf(1) { t[0].len = -1; }

  bool add(char c) {
    int pos = s.size();
    s += c;
    int cur = suf, cl = 0;
    while (true) {
      cl = t[cur].len;
      if (pos - 1 - cl >= 0 && s[pos - 1 - cl] == s[pos]) {
        break;
      }
      cur = t[cur].link;
    }
    if (t[cur][c - 'a']) {
      suf = t[cur][c - 'a'];
      return false;
    }
    int n = t.size();
    t.emplace_back();
    suf = n;
    t[n].len = t[cur].len + 2;
    t[cur][c - 'a'] = n;
    if (t[n].len == 1) {
      t[n].link = 1;
      t[n].cnt = 1;
      return true;
    }
    while (true) {
      cur = t[cur].link;
      cl = t[cur].len;
      if (pos - cl >= 1 and s[pos - 1 - cl] == s[pos]) {
        t[n].link = t[cur][c - 'a'];
        break;
      }
    }
    t[n].cnt = t[t[n].link].cnt + 1;
    return true;
  }
  node operator[](int id) { return t[id]; }
};