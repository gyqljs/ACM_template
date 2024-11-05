#include <bits/stdc++.h>

// 对顶堆

template<class T>
class dhp {
private:
  std::multiset<T> mx;
  std::multiset<T, std::greater<T>> mi;

public:
  T max = 0, min = 0;
  // 插入元素
  void insert(T num) {
    if (!mx.empty() && num > *mx.begin()) {
      max += num;
      mx.insert(num);
      num = *mx.begin();
      max -= *mx.begin();
      mx.erase(mx.begin());
    }
    min += num;
    mi.insert(num);
  }



  void erase(T num) {
    auto it = mx.find(num);
    if (it != mx.end()) {
      mx.erase(it);
      max -= num;
    } else {
      min -= num;
      mi.erase(mi.find(num));
    }
  }


  // 查询第i小的元素
  T findmin_kth(int k) {
    while (mi.size() > k) {
      max += *mi.begin();
      mx.insert(*mi.begin());
      min -= *mi.begin();
      mi.erase(mi.begin());
    }
    while (mi.size() < k) {
      min += *mx.begin();
      mi.insert(*mx.begin());
      max -= *mx.begin();
      mx.erase(mx.begin());
    }
    return *mi.begin();
  }

  // 查询第i大的元素
  T findmax_kth(int k) {
    while (mx.size() > k) {
      mi.insert(*mx.begin());
      mx.erase(mx.begin());
    }
    while (mx.size() < k) {
      mx.insert(*mi.begin());
      mi.erase(mi.begin());
    }
    return *mx.begin();
  }


  bool check(int k) {
    int n = mx.size() + mi.size();
    if (n == 0)
      return 1;
    T mid = findmin_kth((n + 1) / 2);
    int need = max - mx.size() * mid + mi.size() * mid - min;
    if (k >= need)
      return 1;
    else
      return 0;
  }
};


// ST表
template<class T>
class st {
private:
  std::vector<std::vector<T>> arr;

  T op(T a, T b) {
    return std::min(a, b);
  }

public:
  explicit st(const std::vector<T> &v) {
    int len = v.size(), l = ceil(log2(len)) + 1;
    arr.assign(len, std::vector<T>(l, 0));
    for (int i = 0; i < len; i++)
      arr[i][0] = v[i];

    for (int j = 1; j < l; j++) {
      int pj = (1 << (j - 1));
      for (int i = 0; i + pj < len; i++) {
        arr[i][j] = op(arr[i][j - 1], arr[i + (1 << (j - 1))][j - 1]);
      }
    }
  }

  explicit st() {}

  T operator[](int id) { return arr[id][0]; }

  T query(int l, int r) {
    int lt = r - l + 1;
    int q = floor(log2(lt));
    return op(arr[l][q], arr[r - (1 << q) + 1][q]);
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
    int L = l - 1;
    while (l <= r) {
      int m = (l + r) >> 1;
      if (query(l, m) > u)
        r = m - 1;
      else
        l = m + 1, ans = m;
    }
    return ans;
  }
};

// 稀疏表类，用于区间次大值查询
class second_st {
private:
  // 定义一个结构体来存储两个最大值
  struct pii {
    int m1, m2;
    explicit pii(int a = INT32_MIN, int b = INT32_MIN): m1(a), m2(b) {}
  };
  int n, lt; // 最大幂次
  std::vector<std::vector<pii>> st; // 稀疏表
  std::vector<int> log; // 辅助日志表

  // 预处理日志表
  void init_log() {
    log[1] = 0;
    for (int i = 2; i <= n; ++i) {
      log[i] = log[i / 2] + 1;
    }
  }

  // 合并两个Pair，得到新的Pair
  pii merge(const pii &l, const pii &r) {
    int v[4] = {l.m1, l.m2, r.m1, r.m2};
    // 去除无效的次大值
    std::vector<int> vd;
    for (auto val: v) {
      if (val != INT32_MIN) {
        vd.push_back(val);
      }
    }
    // 去重并排序
    sort(vd.begin(), vd.end(), std::greater<int>());
    vd.erase(unique(vd.begin(), vd.end()), vd.end());

    pii p;
    if (!vd.empty()) {
      p.m1 = vd[0];
    }
    if (vd.size() > 1) {
      p.m2 = vd[1];
    }
    return p;
  }

public:
  // 构造函数
  explicit second_st(std::vector<int> &arr): n(arr.size()), log(n + 1) {
    lt = 0;
    while ((1 << (lt + 1)) <= n) lt++;
    st.assign(lt + 1, std::vector<pii>(n));
    init_log();
    for (int i = 0; i < n; ++i) {
      st[0][i] = pii(arr[i], INT32_MIN);
    }
    // 填充其他层
    for (int k = 1; k <= lt; ++k) {
      for (int i = 0; i + (1 << k) <= n; ++i) {
        pii l = st[k - 1][i], r = st[k - 1][i + (1 << (k - 1))];
        st[k][i] = merge(l, r);
      }
    }
  }

  int query(int l, int r) {
    if (l > r || l < 0 || r >= n) {
      // 无效的查询区间
      return INT32_MIN;
    }
    int len = r - l + 1;
    int k = log[len];
    pii lf = st[k][l], rf = st[k][r - (1 << k) + 1];
    // 合并两个Pair
    pii res = merge(lf, rf);
    return res.m2;
  }
};
