#include <bits/stdc++.h>

// 可持久化数组
template<class T>
class persistent_arr {
private:
  struct node {
    int l, r;
    T v;
  };
  std::vector<node> tr;
  std::vector<int> root;
  int n;

  int change(int v, int l, int r, int p, T k) { // 点修
    int u = tr.size();
    tr.push_back(tr[v]); // 动态开点 根据实际情况进行修改
    int mid = (l + r) >> 1;
    if (l == r) {
      tr[u].val = k;
      return u;
    }
    if (p <= mid)
      tr[u].l = change(tr[v].l, l, mid, p, k);
    else
      tr[u].r = change(tr[v].r, mid + 1, r, p, k);
    return u;
  }

  T query(int u, int l, int r, int p) { // 点查
    int mid = (l + r) >> 1;
    if (l == r) return tr[u].val;
    if (p <= mid)
      return query(tr[u].l, l, mid, p);
    else
      return query(tr[u].r, mid + 1, r, p);
  }

public:
  void change(int ver, int pos, T val) {
    root.push_back(change(root[ver], 1, n, pos, val));
  }


  T query(int ver, int pos) {
    T ans = query(root[ver], 1, n, pos);
    root.push_back(root[ver]); // 保留上一个版本
    return ans;
  }

  explicit persistent_arr(std::vector<T> &w): n(w.size()) {
    std::function<int(int, int)> build = [&](int l, int r) {
      int u = tr.size();
      tr.push_back({0, 0, 0}); // 动态开点
      int mid = (l + r) >> 1;
      if (l == r) {
        tr[u].val = w[l];
        return u;
      }
      tr[u].l = build(w, l, mid);
      tr[u].r = build(w, mid + 1, r);
      return u;
    };
    root.push_back(build(1, n));
  }
};


// 可持久化并查集


// 可持久化字典树


// 可持久化平衡树

// 可持久化文艺平衡树
