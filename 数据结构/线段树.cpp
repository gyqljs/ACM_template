#include <bits/stdc++.h>

template<class T>
class seg_tree {
public:
  struct node;
  int n;
  std::vector<node> tr;

  void push_up(node &a, node &x, node &y);


  void push_down(int u, int x, int y) {
    int m = (x + y) >> 1;
    T &lazy = tr[u].tag;
    tr[2 * u].update(x, m, lazy);
    tr[2 * u + 1].update(m + 1, y, lazy);
    lazy = 0;
  }

  void change(int u, int l, int r, int k, int x, int y) {
    if (r < x or l > y)
      return;
    if (l <= x and r >= y) {
      tr[u].update(x, y, k);
      return;
    }
    push_down(u, x, y);
    int m = (x + y) >> 1;
    change(2 * u, l, r, k, x, m);
    change(2 * u + 1, l, r, k, m + 1, y);
    push_up(tr[u], tr[2 * u], tr[2 * u + 1]);
  }

  T query(int u, int l, int r, int x, int y) {
    if (r < x or l > y)
      return (T) 0;
    if (l <= x and y <= r)
      return tr[u].val;
    int m = (x + y) >> 1;
    push_down(u, x, y);
    return query(2 * u, l, r, x, m) + query(2 * u + 1, l, r, m + 1, y);
  }


  void change(int l, int r, int v) {
    change(1, l, r, v, 0, n);
  }

  T query(int l, int r) {
    return query(1, l, r, 0, n);
  }

  int min_pos(T u, int l, int r) {
    int ans = -1;
    while (l <= r) {
      int m = (l + r) >> 1;
      if (query(l, m) < u)
        l = m + 1;
      else
        r = m - 1, ans = m;
    }
    return ans;
  }

  int max_pos(T u, int l, int r) {
    int ans = -1;
    while (l <= r) {
      int m = (l + r) >> 1;
      if (query(l, m) > u)
        r = m - 1;
      else
        l = m + 1, ans = m;
    }
    return ans;
  }


  explicit seg_tree(int n = 0): n(n), tr(4 * n + 4) {}

  explicit seg_tree(std::vector<int> &v): n(v.size()), tr(4 * n) {
    for (int i = 1; i <= n; i++)
      change(i, i, v[i]);
  }
};

struct node {
  int tag;
  //  维护的区间为l ~ r遇到标记的更新函数
  void update(int l, int r, int lazy) {}
};

// 上传函数
node operator+(node &a, node &b);


// 区间+x 区间求和
template<class T>
class add_tree {
public:
  struct node {
    T val = 0, tag = 0;
  };
  int n;
  std::vector<node> tr;

  void push_up(int u) {
    tr[u].val = tr[2 * u].val + tr[2 * u + 1].val;
  }

  void push_down(int u, int x, int y) {
    int m = (x + y) >> 1;
    T &lazy = tr[u].tag;
    tr[2 * u].tag += lazy;
    tr[2 * u + 1].tag += lazy;
    tr[2 * u].val += (m - x + 1) * lazy;
    tr[2 * u + 1].val += (y - m) * lazy;
    lazy = 0;
  }

  void change(int u, int l, int r, int k, int x, int y) {
    if (r < x or l > y)
      return;
    if (l <= x and r >= y) {
      tr[u].val += (y - x + 1) * k;
      tr[u].tag += k;
      return;
    }
    push_down(u, x, y);
    int m = (x + y) >> 1;
    change(2 * u, l, r, k, x, m);
    change(2 * u + 1, l, r, k, m + 1, y);
    push_up(u);
  }

  T query(int u, int l, int r, int x, int y) {
    if (r < x or l > y)
      return (T) 0;
    if (l <= x and y <= r)
      return tr[u].val;
    int m = (x + y) >> 1;
    push_down(u, x, y);
    return query(2 * u, l, r, x, m) + query(2 * u + 1, l, r, m + 1, y);
  }


  void change(int l, int r, int v) {
    change(1, l, r, v, 0, n);
  }

  T query(int l, int r) {
    return query(1, l, r, 0, n);
  }

  explicit add_tree(int n = 0): n(n), tr(4 * n + 4) {}

  explicit add_tree(std::vector<int> &v): n(v.size()), tr(4 * n) {
    for (int i = 1; i <= n; i++)
      change(i, i, v[i]);
  }
};

// 区间+x 区间最值查询
template<class T>
class rmq_tree {
public:
  struct node {
    T val = 0, tag = 0;
  };
  int n;
  std::vector<node> tr;

  void push_up(int u) {
    tr[u].val = std::max(tr[2 * u].val, tr[2 * u + 1].val);
  }

  void push_down(int u, int x, int y) {
    T &lazy = tr[u].tag;
    tr[2 * u].tag += lazy, tr[2 * u + 1].tag += lazy;
    tr[2 * u].val += lazy;
    tr[2 * u + 1].val += lazy;
    lazy = 0;
  }

  void change(int u, int l, int r, int k, int x, int y) {
    if (r < x or l > y)
      return;
    if (l <= x and r >= y) {
      tr[u].val += k;
      tr[u].tag += k;
      return;
    }
    push_down(u, x, y);
    int m = (x + y) >> 1;
    change(2 * u, l, r, k, x, m);
    change(2 * u + 1, l, r, k, m + 1, y);
    push_up(u);
  }

  T query(int u, int l, int r, int x, int y) {
    if (r < x or l > y)
      return (T) -1e18;
    if (l <= x and y <= r)
      return tr[u].val;
    int m = (x + y) >> 1;
    push_down(u, x, y);
    return query(2 * u, l, r, x, m) + query(2 * u + 1, l, r, m + 1, y);
  }


  void change(int l, int r, int v) {
    change(1, l, r, v, 0, n);
  }

  T query(int l, int r) {
    return query(1, l, r, 0, n);
  }

  explicit rmq_tree(int n = 0): n(n), tr(4 * n + 4) {}

  explicit rmq_tree(std::vector<int> &v): n(v.size()), tr(4 * n + 4) {
    for (int i = 0; i <= n; i++)
      change(i, i, v[i]);
  }
};


class xor_tree {
public:
  struct node {
    int tag, w[20];
  };
  int n;
  std::vector<node> t;


  void push_down(int u, int x, int y) {
    int &lazy = t[u].tag, m = (x + y) / 2;
    for (int i = 0; i < 20; i++) {
      if ((lazy >> i) & 1) {
        t[2 * u].w[i] = (m - x + 1) - t[2 * u].w[i];
        t[2 * u + 1].w[i] = (y - m) - t[2 * u + 1].w[i];
      }
    }
    t[2 * u].tag ^= lazy, t[2 * u + 1].tag ^= lazy;
    lazy = 0;
  }

  void push_up(int u) {
    for (int i = 0; i < 20; i++)
      t[u].w[i] = t[2 * u].w[i] + t[2 * u + 1].w[i];
  }

  void change(int u, int l, int r, int val, int x, int y) {
    if (r < x or l > y)
      return;
    if (l <= x and y <= r) {
      for (int i = 0; i < 20; i++)
        if ((val >> i) & 1)
          t[u].w[i] = y - x + 1 - t[u].w[i];
      t[u].tag ^= val;
      return;
    }
    push_down(u, x, y);
    int m = (x + y) / 2;
    change(2 * u, l, r, val, x, m);
    change(2 * u + 1, l, r, val, m + 1, y);
    push_up(u);
  }

  int64_t query(int u, int l, int r, int x, int y) {
    if (r < x or l > y)
      return 0;
    if (l <= x and y <= r) {
      int64_t ans = 0;
      for (int i = 0; i < 20; i++) {
        ans += t[u].w[i] * (1LL << i);
      }
      return ans;
    }
    push_down(u, x, y);
    int m = (x + y) / 2;
    return query(2 * u, l, r, x, m) + query(2 * u + 1, l, r, m + 1, y);
  }

  void change(int l, int r, int v) {
    change(1, l, r, v, 0, n);
  }

  int64_t query(int l, int r) {
    return query(1, l, r, 0, n);
  }

  xor_tree(int n = 0): n(n), t(4 * n + 4) {}

  xor_tree(std::vector<int> &v): n(v.size()), t(4 * n + 4) {
    for (int i = 0; i <= n; i++)
      change(i, i, v[i]);
  }
};

// 区间加乘
template<class T>
struct add_mul_tree {
  int n;
  struct node {
    T v, add, mul = 1; // 注意初始赋值
  };
  std::vector<node> t;


  void push_down(int u, int x, int y) { // 不需要动
    int m = (x + y) >> 1;
    T &add = t[u].add, &mul = t[u].mul;
    t[2 * u].v = t[2 * u].v * mul + (m - x + 1) * add;
    t[2 * u].add = t[2 * u].add * mul + add;
    t[2 * u].mul *= mul;
    t[2 * u + 1].v = t[2 * u + 1].v * mul + (y - m) * add;
    t[2 * u + 1].add = t[2 * u + 1].add * mul + add;
    t[2 * u + 1].mul *= mul;
    add = 0, mul = 1;
  }

  void push_up(int k) { // 不需要动
    t[k].v = t[2 * k].v + t[2 * k + 1].v;
  }

  void add(int u, int l, int r, T val, int x, int y) {
    if (r < x or l > y)
      return;
    if (l <= x and y <= r) {
      t[u].v += (y - x + 1) * val;
      t[u].add += val;
      return;
    }
    push_down(u, x, y);
    int m = (x + y) >> 1;
    add(2 * u, l, r, val, x, m);
    add(2 * u + 1, l, r, val, m + 1, y);
    push_up(u);
  }

  void mul(int u, int l, int r, T val, int x, int y) { // 区间修改
    if (r < x or l > y)
      return;
    if (l <= x and y <= r) {
      t[u].v *= val;
      t[u].add *= val;
      t[u].mul *= val;
      return;
    }
    push_down(u, x, y);
    int m = (x + y) >> 1;
    mul(2 * u, l, r, val, x, m);
    mul(2 * u + 1, l, r, val, m + 1, y);
    push_up(u);
  }

  T query(int u, int l, int r, int x, int y) { // 区间询问，不合并
    if (r < x or l > y)
      return (T) 0;
    if (l <= x and y <= r) {
      return t[u].v;
    }
    push_down(u, x, y);
    int m = (x + y) >> 1;
    return query(2 * u, l, r, x, m) + query(2 * u + 1, l, r, m + 1, y);
  }


  void add(int l, int r, int v) {
    add(1, l, r, v, 0, n);
  }

  void mul(int l, int r, int v) {
    mul(1, l, r, v, 0, n);
  }

  T query(int l, int r) {
    return query(1, l, r, 0, n);
  }

  explicit add_mul_tree(int n = 0, int mod = 1999999): n(n), t(4 * n + 4) {}
};
