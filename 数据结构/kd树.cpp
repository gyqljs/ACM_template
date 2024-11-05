#include <bits/stdc++.h>


// int K, root; // K 维度, root 根, cur 当前节点

template<class T, size_t K = 2>
class kd_tree {
public:
  const double A = 0.7; // 重构常数

  using poi = std::array<T, K>;

  struct node {
    int l, r; // 左右孩子
    std::array<T, 2> v, mi, mx;
    int sz; // 子树大小
    T &operator[](int id) { return v[id]; }
    explicit node(){};
    explicit node(poi &a) { v = a; }
  };

  int root = 0;

  std::vector<node> tr; // KD 树节点信息
  std::vector<int> g; // 存储节点的编号


  void pushup(int p) { // 更新 p 子树的信息
    tr[p].sz = (tr[p].l ? tr[tr[p].l].sz : 0) + (tr[p].r ? tr[tr[p].r].sz : 0) + 1;
    for (int i = 0; i < 2; i++) {
      tr[p].mi[i] = tr[p].mx[i] = tr[p].v[i];
      if (tr[p].l) {
        tr[p].mi[i] = std::min(tr[p].mi[i], tr[tr[p].l].mi[i]);
        tr[p].mx[i] = std::max(tr[p].mx[i], tr[tr[p].l].mx[i]);
      }
      if (tr[p].r) {
        tr[p].mi[i] = std::min(tr[p].mi[i], tr[tr[p].r].mi[i]);
        tr[p].mx[i] = std::max(tr[p].mx[i], tr[tr[p].r].mx[i]);
      }
    }
  }


  int rebuild(int l, int r, int k) { // 重构子树
    if (l > r) return 0;
    int m = (l + r) / 2;
    // K = k;
    auto cmp = [&](int a, int b) { // 按点的坐标值比较
      return tr[a].v[k] < tr[b].v[k];
    };
    std::nth_element(g.begin() + l, g.begin() + m, g.begin() + r + 1, cmp);
    tr[g[m]].l = rebuild(l, m - 1, k ^ 1);
    tr[g[m]].r = rebuild(m + 1, r, k ^ 1);
    pushup(g[m]);
    return g[m];
  }

  void dfs(int p) { // 提取 p 子树的节点编号
    if (!p) return;
    g.push_back(p);
    dfs(tr[p].l);
    dfs(tr[p].r);
  }

  void check(int &p, int k) { // 检查
    if (A * tr[p].sz < std::max((tr[p].l ? tr[tr[p].l].sz : 0), (tr[p].r ? tr[tr[p].r].sz : 0))) {
      g.clear();
      dfs(p);
      p = rebuild(0, g.size() - 1, k);
    }
  }

  void insert(int cur, int &p, int k) { // 插点
    if (!p) {
      p = cur;
      pushup(p);
      return;
    }
    insert(cur, tr[cur].v[k] <= tr[p].v[k] ? tr[p].l : tr[p].r, k ^ 1);
    pushup(p);
    check(p, k); // 检查是否重构 p 子树
  }

  double ans = 2e18;

  double sq(double x) { return x * x; }

  double dis(int p, int q) { // 当前点到 p 点的距离
    double s = 0;
    for (int i = 0; i < 2; i++) s += sq(tr[q].v[i] - tr[p].v[i]);
    return s;
  }

  double dis2(int a, int p) { // 当前点到 p 子树区域的最小距离
    if (!p) return 2e18;
    double s = 0;
    for (int i = 0; i < 2; ++i)
      s += sq(std::max(tr[a].v[i] - tr[p].mx[i], 0.0)) + sq(std::max(tr[p].mi[i] - tr[a].v[i], 0.0));
    return s;
  }

  void query(int a, int p) {
    if (!p) return;
    if (p != a) ans = std::min(ans, dis(p, a));
    double dl = dis2(a, tr[p].l), dr = dis2(a, tr[p].r);
    if (dl < dr) {
      if (dl < ans) query(a, tr[p].l);
      if (dr < ans) query(a, tr[p].r);
    } else {
      if (dr < ans) query(a, tr[p].r);
      if (dl < ans) query(a, tr[p].l);
    }
  }

  void insert(poi &a) {
    tr.emplace_back(a);
    insert(tr.size() - 1, root, 0);
  }

  T query(int id) {
    query(id, root);
    return ans;
  }

  void work() {
    g.resize(tr.size());
    std::iota(g.begin(), g.end(), 0);
    // 插入功能拓展
    root = rebuild(1, tr.size() - 1, 0);
  }


  explicit kd_tree(int n = 0): tr(n + 1) {}

  node &operator[](int id) { return tr[id]; }
};
