#include <bits/stdc++.h>

template<class T>
class Hjt_tree {
public:
  struct node {
    int l = 0, r = 0;
    T v = 0;
  };

  int n, cnt = 0;
  std::vector<node> tr;
  std::vector<int> root;



  // Change function now returns the new index
  void update(int &u, int v, int l, int r, int p) {
    u = ++cnt;
    tr[u] = tr[v];
    tr[u].v++;
    if (l == r) {
      return;
    }
    int m = (l + r) >> 1;
    if (p <= m)
      update(tr[u].l, tr[v].l, l, m, p);
    else
      update(tr[u].r, tr[v].r, m + 1, r, p);
  }

  T kth(int u, int v, int l, int r, int k) {
    if (l == r)
      return l;
    int m = (l + r) / 2;
    T s = tr[tr[u].l].v - tr[tr[v].l].v;
    if (k <= s)
      return kth(tr[u].l, tr[v].l, l, m, k);
    else
      return kth(tr[u].r, tr[v].r, m + 1, r, k - s);
  }

  T query(int u, int l, int r, int x, int y) {
    if (x > r or y < l)
      return 0;
    if (x <= l and r <= y)
      return tr[u].v;
    int m = (l + r) >> 1;
    return query(tr[u].l, l, m, x, y) + query(tr[u].r, m + 1, r, x, y);
  }



  explicit Hjt_tree(std::vector<int> &w): n(w.size()), tr((log2(n) + 4) * n), root(n) {
    for (int i = 0; i < n; i++) {
      update(root[i + 1], root[i], 1, n, w[i]);
    }
  }

  explicit Hjt_tree(int n, int m): n(n), tr(log2(n) * m + 4 * n), root(1) {}

  void update(T k, int p = -1) {
    p = p == -1 ? root.size() - 1 : p;
    root.emplace_back();
    update(root.back(), root[p], 1, n, k);
  }

  T kth(int l, int r, int k) {
    return kth(root[r], root[l - 1], 1, n, k);
  }

  // tr.query(r, b, d) - tr.query(l - 1, b, d) 查询l ~ r 区间有多少数字在b ~ d之间
  T query(int p, int x, int y) {
    return query(root[p], 1, n, x, y);
  }
};



template<class T>
class wseg_forest {
public:
  struct node {
    int l = 0, r = 0;
    T v = 0;
  };
  int n;
  std::vector<node> tr;
  std::vector<int> root;

  explicit wseg_forest(int n, int sz = 1): n(n), tr(1), root(sz + 1) {}

  void merge_(int &x, int y) {
    if (!x or !y) {
      x = x + y;
      return;
    }
    tr[x].v += tr[y].v;
    merge_(tr[x].l, tr[y].l);
    merge_(tr[x].r, tr[y].r);
  }

  void split_(int x, int &y, T k) { // 分裂
    // 将x从k处斩断，前k小的归x，后面的归y
    if (tr[x].v == x)
      return;
    y = tr.size(); // 开点
    tr.emplace_back();
    int64_t s = tr[tr[x].l].v;
    if (k <= s)
      split_(tr[x].l, tr[y].l, k), std::swap(tr[x].r, tr[y].r);
    else
      split_(tr[x].r, tr[y].r, k - s);

    tr[y].v = tr[x].v - k, tr[x].v = k;
  }


  void change_(int &u, int l, int r, int p, int k) { // 点修
    if (!u) {
      u = tr.size(); // 开点
      tr.emplace_back();
    }
    tr[u].v += k;
    if (l == r) return;
    int m = (l + r) >> 1;
    if (p <= m)
      change_(tr[u].l, l, m, p, k);
    else
      change_(tr[u].r, m + 1, r, p, k);
  }

  T query_(int u, int l, int r, int x, int y) {
    if (x > r or y < l)
      return 0;
    if (x <= l and r <= y)
      return tr[u].v;
    int m = (l + r) >> 1;
    return query_(tr[u].l, l, m, x, y) + query_(tr[u].r, m + 1, r, x, y);
  }

  int kth_(int u, int l, int r, int k) { // 第k小
    if (l == r) return l;
    int m = (l + r) >> 1;
    if (k <= tr[tr[u].l].v)
      return kth_(tr[u].l, l, m, k);
    else
      return kth_(tr[u].r, m + 1, r, k - tr[tr[u].l].v);
  }


  void spilt(int p, int x, int y) {
    T k1 = query_(root[p], 1, n, 1, y);
    T k2 = query_(root[p], 1, n, x, y);
    int tmp = 0;
    root.push_back(0);
    split_(root[p], root.back(), k1 - k2);
    split_(root.back(), tmp, k2);
    merge_(root[p], tmp);
  }

  void merge(int x, int y) {
    merge_(root[x], root[y]);
  }


  void change(int p, int x, int y) {
    change_(root[p], 1, n, y, x);
  }

  T query(int p, int x, int y) {
    return query_(root[p], 1, n, x, y);
  }

  int kth(int p, int k) {
    if (tr[root[p]].v < k)
      return -1;
    return kth_(root[p], 1, n, k);
  }
};
