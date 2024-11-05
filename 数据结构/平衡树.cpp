#include <bits/stdc++.h>

template<class T>
class treap {
public:
  struct node {
    uint64_t k;
    node *l, *r;
    int sz;
    T val;

    explicit node(T v): k(rand()), l(nullptr), r(nullptr), sz(1), val(v) {}
  };

  node *root = nullptr;

  void push_up(node *n) {
    n->sz = (n->l ? n->l->sz : 0) + (n->r ? n->r->sz : 0) + 1;
  }

  void split(node *r, T k, node *&x, node *&y) {
    if (!r) {
      x = y = nullptr;
      return;
    }
    if (r->val <= k) {
      x = r;
      split(r->r, k, r->r, y);
    } else {
      y = r;
      split(r->l, k, x, r->l);
    }
    push_up(r);
  }

  node *merge(node *x, node *y) {
    if (!x || !y)
      return x ? x : y;
    if (x->k <= y->k) {
      x->r = merge(x->r, y);
      push_up(x);
      return x;
    } else {
      y->l = merge(x, y->l);
      push_up(y);
      return y;
    }
  }

  T kth(node *n, int k) {
    int lsz = n->l ? n->l->sz : 0;
    if (lsz + 1 == k)
      return n->val;
    if (lsz >= k)
      return kth(n->l, k);
    else
      return kth(n->r, k - lsz - 1);
  }


  void insert(T k) {
    node *x, *y;
    split(root, k, x, y);
    root = merge(merge(x, new node(k)), y);
  }

  void erase(T k) {
    node *x, *y, *z;
    split(root, k, x, y);
    split(x, k - 1, x, z);
    //   node *td = z; // 保存要删除的节点的指针
    z = merge(z->l, z->r);
    //  delete td; // 删除节点，释放内存
    root = merge(merge(x, z), y);
  }

  T get_val(int k) {
    if (k <= 0)
      return -1;
    if (root->sz < k)
      return -1;
    return kth(root, k);
  }

  // 查询k是第几小的
  int get_rk(T k) {
    node *x, *y;
    split(root, k - 1, x, y);
    int ans = (x ? x->sz : 0) + 1;
    root = merge(x, y);
    return ans;
  }

  T pre(T k) {
    node *x, *y;
    split(root, k - 1, x, y);
    if (x == nullptr)
      return INT32_MIN + 1;
    T ans = kth(x, x->sz);
    root = merge(x, y);
    return ans;
  }

  T suf(T k) {
    node *x, *y;
    split(root, k, x, y);
    if (y == nullptr)
      return INT32_MAX;
    T ans = kth(y, 1);
    root = merge(x, y);
    return ans;
  }

  size_t size() { return root->sz; }
};

// 文艺平衡树
class spaly {
public:
  struct node {
    uint64_t k = 0;
    int l = 0, r = 0, sz = 0, tag = 0;
    int val;
    node(int v = 0): val(v) { k = rand(); }
  };

  std::vector<node> t;

  void adde(int x) {
    t[x].val = x;
    t[x].sz = 1;
    t[x].k = rand();
    t[x].l = t[x].r = 0;
  }

  void check_tag(int u) {
    if (!t[u].tag)
      return;
    std::swap(t[u].l, t[u].r); // 交换节点
    // 下传懒标记
    t[t[u].l].tag ^= 1;
    t[t[u].r].tag ^= 1;
    t[u].tag = 0; // 清除标记
  }

  void update(int u) { // 更新操作
    t[u].sz = t[t[u].l].sz + t[t[u].r].sz + 1;
  }

  // x，y为左右子树的根节点
  void split(int rt, int k, int &x, int &y) {
    if (!rt) {
      x = y = 0;
      return;
    }
    check_tag(rt);
    if (t[t[rt].l].sz + 1 <= k) {
      x = rt;
      split(t[rt].r, k - t[t[rt].l].sz - 1, t[rt].r, y);
    } else {
      y = rt;
      split(t[rt].l, k, x, t[rt].l);
    }
    update(rt);
  }

  // 合并以x,y为根节点的左右子树
  int merge(int x, int y) {
    if (!x || !y)
      return x + y;
    // 按照键值维护一个小根堆
    if (t[x].k < t[y].k) {
      // l当父节点
      check_tag(x);
      t[x].r = merge(t[x].r, y); // 确定l的右儿子
      update(x);
      return x;
    } else {
      // r当父节点
      check_tag(y);
      t[y].l = merge(x, t[y].l); // 确定r的左儿子
      update(y);
      return y;
    }
  }

  int root = 0;
  void print(int u) {
    check_tag(u);
    if (t[u].l)
      print(t[u].l);
    printf("%d ", t[u].val);
    if (t[u].r)
      print(t[u].r);
  }

  void rev(int x, int y) {
    int l, r, p;
    // 将树分为[l,p]和[r]
    split(root, y, l, r);
    // 将[l,m]分为[l,x-1]和[x,p]
    split(l, x - 1, l, p);
    t[p].tag ^= 1;
    root = merge(merge(l, p), r); // 合并
  }

  explicit spaly(int n): t(n + 1) {
    for (int i = 1; i <= n; i++) {
      adde(i);
      root = merge(root, i);
    }
  }
};

// 平衡树森林
template<class T>
class treap_forest {
public:
  int n;
  std::vector<treap<T>> tr;
  std::vector<int> p;


  void dfs(class treap<T>::node *x, int &y) {
    if (x->l != nullptr)
      dfs(x->l, y);
    if (x->r != nullptr)
      dfs(x->r, y);
    tr[y].insert(x->val);
  }

  int find(int x) {
    if (p[x] != x)
      p[x] = find(p[x]);
    return p[x];
  }


  void merge(int x, int y) {
    x = find(x), y = find(y);
    if (x != y) {
      if (tr[x].root->sz > tr[y].root->sz)
        std::swap(x, y);
      p[x] = y;
      dfs(tr[x].root, y);
    }
  }

  explicit treap_forest(int n): tr(n), p(n) {
    for (int i = 0; i < n; i++)
      p[i] = i;
  }

  treap<T> &operator[](int id) { return tr[find(id)]; }
};


// 线段树套平衡树
template<typename T>
struct seg_treap {
  int n;
  std::vector<treap<T>> tr;
  std::vector<T> w;

  int get_rk_(int u, int l, int r, int x, int y, int v) {
    if (x <= l and r <= y)
      return tr[u].get_rk(v) - 1;
    int m = (l + r) >> 1, ans = 0;
    if (x <= m)
      ans += get_rk_(2 * u, l, m, x, y, v);
    if (y > m)
      ans += get_rk_(2 * u + 1, m + 1, r, x, y, v);
    return ans;
  }

  int get_rk(int l, int r, int v) {
    return get_rk_(1, 1, n, l, r, v) + 1;
  }

  T get_val(int x, int y, int k) {
    int l = 0, r = 1e8 + 2, ans = 0;
    while (l <= r) {
      int m = (l + r) >> 1;
      if (get_rk_(1, 1, n, x, y, m) + 1 <= k) {
        ans = m, l = m + 1;
      } else
        r = m - 1;
    }
    return ans;
  }


  void change_(int u, int l, int r, int p, int v) {
    tr[u].erase(w[p]);
    tr[u].insert(v);
    if (l == r)
      return;
    int m = (l + r) >> 1;
    if (p <= m)
      change_(2 * u, l, m, p, v);
    else
      change_(2 * u + 1, m + 1, r, p, v);
  }

  void change(int pos, int val) {
    change_(1, 1, n, pos, val);
    w[pos] = val;
  }

  T pre_(int u, int l, int r, int x, int y, int v) {
    if (x <= l and r <= y)
      return tr[u].pre(v);
    int m = (l + r) >> 1, res = INT32_MIN + 1;
    if (x <= m)
      res = std::max(res, pre_(2 * u, l, m, x, y, v));
    if (y > m)
      res = std::max(res, pre_(2 * u + 1, m + 1, r, x, y, v));
    return res;
  }

  T pre(int l, int r, int k) {
    return pre_(1, 1, n, l, r, k);
  }

  T suf_(int u, int l, int r, int x, int y, int v) {
    if (x <= l and r <= y)
      return tr[u].suf(v);
    int m = (l + r) >> 1, res = INT32_MAX;
    if (x <= m)
      res = std::min(res, suf_(2 * u, l, m, x, y, v));
    if (y > m)
      res = std::min(res, suf_(2 * u + 1, m + 1, r, x, y, v));
    return res;
  }

  T suf(int l, int r, int k) {
    return suf_(1, 1, n, l, r, k);
  }

  void build(int u, int l, int r) {
    for (int i = l; i <= r; i++)
      tr[u].insert(w[i]);
    if (l == r)
      return;
    int m = (l + r) >> 1;
    build(2 * u, l, m);
    build(2 * u + 1, m + 1, r);
  };

  void work() { build(1, 1, n); }

  explicit seg_treap(int n): n(n - 1), w(n), tr(4 * n) {}

  int &operator[](int id) { return w[id]; }
};
