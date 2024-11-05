#include <bits/stdc++.h>

// 莫队是一种基于分块思想的离线算法，用于解决区间问题，适用范围如下：
// 只有询问没有修改。
// 允许离线。
// 在已知询问[l, r]答案的情况下可以O(1)得到[l, r - 1], [l, r + 1], [l - 1, r], [l + 1, r]的答案
class mo_team {
public:
  void add(int x);

  void del(int x);

  int len;

  struct query {
    int l, r, id;
  };

  std::vector<query> q;
  std::vector<int> a;
  int sum = 0;

  void work() {
    auto cmp = [&](const query &x, const query &y) -> bool {
      if (x.l / len != y.l / len)
        return x.l / len < y.l / len;
      return x.r < y.r;
    };
    std::sort(q.begin(), q.end(), cmp);

    std::vector<int> ans(q.size());
    int l = 1, r = 0;

    for (int i = 0; i < q.size(); i++) {
      while (l > q[i].l)
        add(a[--l]);
      while (r < q[i].r)
        add(a[++r]);
      while (l < q[i].l)
        del(a[l++]);
      while (r > q[i].r)
        del(a[r--]);
      ans[q[i].id] = sum;
    }

    for (int i = 0; i < q.size(); i++)
      std::cout << ans[i] << '\n';
  }

  explicit mo_team(int n): a(n), len(sqrt(n)) {}
};

std::unordered_map<int, int> buc;
void mo_team::add(int x) {
  sum += 2 * buc[x] + 1;
  buc[x]++;
};

void mo_team::del(int x) {
  sum -= 2 * buc[x] - 1;
  buc[x]--;
};


class change_mo_team {
public:
  struct change {
    int p, col;
  };
  struct query {
    int l, r, t, id;
  };

  int len;


  std::vector<change> c;
  std::vector<query> q;
  std::vector<int> a;

  void add(int x);

  void del(int x);

  void modify(int x, int ti, query &cur);


  explicit change_mo_team(int n): a(n), len(pow(n - 1, 0.66666)) {}


  int sum = 0;
  void work() {
    auto cmp = [&](query a, query b) {
      if (a.l / len != b.l / len)
        return a.l / len < b.l / len;
      if (a.r / len != b.r / len)
        return a.r / len < b.r / len;
      return a.t < b.t;
    };
    sort(q.begin(), q.end(), cmp);

    int l = 2, r = 1, now = 0;
    std::vector<int> out(q.size());

    for (int i = 0; i < q.size(); i++) {
      while (l > q[i].l)
        add(a[--l]);
      while (r < q[i].r)
        add(a[++r]);
      while (l < q[i].l)
        del(a[l++]);
      while (r > q[i].r)
        del(a[r--]);

      // 应用或回滚修改以匹配当前时间
      while (now < q[i].t)
        modify(i, now++, q[i]);
      while (now > q[i].t)
        modify(i, --now, q[i]);

      out[q[i].id] = sum;
    }

    for (int i = 0; i < q.size(); ++i) {
      std::cout << out[i] << "\n";
    }
  }
};

int cnt[1000005];

void change_mo_team::add(int x) {
  if (cnt[x]++ == 0) {
    ++sum;
  }
}

void change_mo_team::del(int x) {
  if (--cnt[x] == 0) {
    --sum;
  }
}

void change_mo_team::modify(int x, int ti, change_mo_team::query &cur) {
  if (c[ti].p >= cur.l && c[ti].p <= cur.r) {
    del(a[c[ti].p]);
    add(c[ti].col);
  }
  std::swap(a[c[ti].p], c[ti].col);
}

class repair_mo_team {
public:
  struct query {
    int l, r, id;
  };
  int len;
  std::vector<int64_t> a, cnt, block;
  std::vector<query> q;

  int64_t add(int x);

  void del(int x);

  int64_t res = 0, last = 0;
  void work() {
    int n = a.size() - 1, m = q.size();
    std::vector<int> ans(q.size());

    for (int i = 1; i <= n; i++)
      block[i] = (i - 1) / len;

    // 自定义比较函数，用于莫队排序
    auto cmp = [&](const query &a, const query &b) -> bool {
      if (block[a.l] != block[b.l])
        return block[a.l] < block[b.l];
      return a.r < b.r;
    };
    std::sort(q.begin(), q.end(), cmp);

    int num = block[n] + 1; // 块数，索引从0开始


    int x = 0; // 查询索引从0开始
    for (int i = 0; i < num; i++) { // 第i块，索引从0开始
      res = 0, last = 0;
      fill(cnt.begin(), cnt.end(), 0LL); // 清空cnt
      int end = std::min(len * (i + 1), n);
      int l = end + 1, r = end;
      while (x < m && block[q[x].l] == i) { // 第i块的查询x
        if (block[q[x].l] == block[q[x].r]) { // 块内
          // 计算块内答案
          ans[q[x].id] = add(x);
          x++;
          continue;
        }
        // 处理跨块的查询
        while (r < q[x].r) {
          r++;
          del(r);
        }
        last = res; // 结果存为last
        while (l > q[x].l) {
          l--;
          del(l);
        }
        ans[q[x].id] = res; // 结果存入答案
        while (l <= end) {
          cnt[a[l]]--;
          l++;
        } // 回滚l
        res = last; // 回滚结果
        x++;
      }
    }

    // 输出答案，按照查询的原始顺序
    for (int i = 0; i < m; ++i) {
      std::cout << ans[i] << "\n";
    }
  }

  explicit repair_mo_team(int n): len(sqrt(n)), a(n + 1), cnt(n + 1), block(n + 1) {}
};

std::vector<int> b;
int64_t repair_mo_team::add(int x) {
  int64_t mx = 0;
  std::vector<int> c(a.size());
  for (int j = q[x].l; j <= q[x].r; j++) {
    ++c[a[j]];
    mx = std::max(mx, (int64_t) c[a[j]] * b[a[j]]);
  }
  return mx;
}

void repair_mo_team::del(int x) {
  cnt[a[x]]++;
  res = std::max(res, cnt[a[x]] * b[a[x]]);
}
