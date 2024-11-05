#include <bits/stdc++.h>


template<class T>
class lichao_tree {
private:
  using line = std::pair<T, T>;
  int n;
  std::vector<line> tree;

  static T eva(const line &s, int x) {
    return s.first * static_cast<T>(x) + s.second;
  }

  void add_line(line s, int u, int l, int r) {
    int m = (r + l) / 2;

    if (eva(s, m) < eva(tree[u], m)) {
      std::swap(tree[u], s);
    }

    if (eva(s, l) < eva(tree[u], l)) {
      add_line(s, 2 * u, l, m);
    }
    if (eva(s, r) < eva(tree[u], r)) {
      add_line(s, 2 * u + 1, m + 1, r);
    }
  }

  T query_min(int x, int u, int l, int r) const {
    if (l == r) {
      return eva(tree[u], x);
    }
    int m = (r + l) / 2;
    T res = eva(tree[u], x);
    if (x <= m) {
      res = std::min(res, query_min(x, 2 * u, l, m));
    } else {
      res = std::min(res, query_min(x, 2 * u + 1, m + 1, r));
    }
    return res;
  }

public:
  explicit lichao_tree(int n): n(n), tree(4 * n + 4, std::make_pair(0, 2e18)) {}

  void add(line u) {
    add_line(u, 1, 0, n);
  }

  T get_min(int x) {
    return query_min(x, 1, 0, n);
  }
};