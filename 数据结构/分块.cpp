#include <bits/stdc++.h>


// 定义 Block 类
template<class T>
class block {
public:
  T val = 0, tag = 0; // 块的懒惰标记
  int l, r, id;
  std::vector<T> e;

  // 构造函数
  explicit block(int id, int l, int r): id(id), l(l), r(r) {}

  // 构造函数
  explicit block(int id, int l, int r, std::vector<T> &arr): id(id), l(l), r(r) {
    e.reserve(r - l + 1);
    for (int i = l; i <= r; i++) {
      e.push_back(arr[i]);
      val += arr[i];
    }
  }

  void convert(int &x, int &y) { x -= l, y -= l; }

  // 获取块内区间 [l, r] 的元素和
  // 这里的 l 和 r 是全局索引，需要转换为块内索引
  T query(int l, int r) {
    convert(l, r);

    T res = 0;
    for (int i = l; i <= r; ++i) {
      res += e[i]; // 已经应用了懒惰标记，所以不需要加 lazy
    }
    return res;
  }

  T query() { return val + tag * e.size(); }

  // 将块内区间 [l, r] 的每个元素加上 k
  // 这里的 l 和 r 是全局索引，需要转换为块内索引
  void change(int l, int r, T k) {
    convert(l, r);

    for (int i = l; i <= r; ++i) {
      e[i] += k;
      val += k;
    }
  }

  // 应用懒惰标记到具体元素
  void apply_lazy() {
    if (tag != 0) {
      for (auto &x: e) {
        x += tag;
      }
      val += tag * e.size();
      tag = 0;
    }
  }

  T &operator[](int id) { return e[id]; }
};


template<class T>
bool operator<(int a, block<T> b) { return a < b.l; }


template<class T>
class block_array {
private:
  std::vector<block<T>> b; // 所有块

  // 通过二分搜索找到索引 id 所在的块
  int get_id(int id) {
    // 然后减一得到包含 id 的块
    int idx = upper_bound(b.begin(), b.end(), id) - b.begin() - 1;
    // 确保 idx 在合法范围内
    if (idx < 0) idx = 0;
    return idx;
  }

public:
  explicit block_array(std::vector<T> &arr) {
    int n = arr.size();
    int sp = ceil(sqrt(n));
    int cnt = (n + sp - 1) / sp;
    b.reserve(cnt);

    // 初始化每个块
    for (int i = 0; i < cnt; ++i) {
      int l = i * sp;
      int r = std::min((i + 1) * sp - 1, n - 1);
      b.emplace_back(i, l, r, arr);
    }
  }

  void add(int sz) {
    int l = b.empty() ? 0 : b.back().r + 1;
    int r = l + sz - 1;
    b.emplace_back(b.size(), l, r);
  }

  void change(int x, int y, T k) {
    int ql = get_id(x), qr = get_id(y);

    if (ql == qr) {
      // 同一个块，先应用懒惰标记，然后逐个元素更新
      b[ql].apply_lazy();
      b[ql].change(x, y, k);
    } else {
      // 更新起始块的部分元素
      b[ql].apply_lazy();
      b[ql].change(x, b[ql].r, k);

      // 更新中间完整块，使用懒惰标记
      for (int i = ql + 1; i <= qr - 1; i++) {
        b[i].tag += k;
      }

      // 更新结束块的部分元素
      b[qr].apply_lazy();
      b[qr].change(b[qr].l, y, k);
    }
  }

  T query(int x, int y) {
    T ans = 0;
    int ql = get_id(x), qr = get_id(y);

    if (ql == qr) {
      // 同一个块，先应用懒惰标记，然后逐个元素计算
      b[ql].apply_lazy();
      ans += b[ql].query(x, y);
    } else {
      // 计算起始块的部分元素和
      b[ql].apply_lazy();
      ans += b[ql].query(x, b[ql].r);
      // 计算中间完整块的和
      for (int i = ql + 1; i <= qr - 1; i++) {
        ans += b[i].query();
      }
      // 计算结束块的部分元素和
      b[qr].apply_lazy();
      ans += b[qr].query(b[qr].l, y);
    }

    return ans;
  }
};
