#include <bits/stdc++.h>

struct node {
  int x, y, z, id;
};


template<class T>
struct fenw {
private:
  int n;
  std::vector<T> s;

public:
  void add(int p, int x) {
    while (p <= n) {
      s[p] += x;
      p += p & -p; // 因为是添加应该从下至上,故不停+lowbit
    }
  }

  int cal(int p) {
    int ans = 0;
    while (p > 0) {
      ans += s[p];
      p -= p & -p; // 因为是查询所以应该从上至下，故不停地-lowbit 进行统计
    }
    return ans;
  }

  int ask(int l, int r) {
    return cal(r) - cal(l - 1);
  }

  int select(const T &k) {
    int x = 0;
    T cur{};
    for (int i = 1 << (int) std::log2(n); i; i /= 2) {
      if (x + i <= n and cur + s[x + i - 1] <= k) {
        x += i;
        cur = cur + s[x - 1];
      }
    }
    return x;
  }

  explicit fenw(int n = 0): n(n - 1), s(n) {}

  explicit fenw(std::vector<T> &a): n(a.size() - 1), s(a) {
    for (int i = 1; i <= n; ++i) {
      int j = i + (i & -i);
      if (j <= n)
        s[j] += s[i];
    }
  }
};


bool cmp1(const node &a, const node &b) {
  if (a.x != b.x)
    return a.x < b.x;
  if (a.y != b.y)
    return a.y < b.y;
  return a.z < b.z;
}

bool cmp2(const node &a, const node &b) {
  if (a.y != b.y)
    return a.y < b.y;
  if (a.z != b.z)
    return a.z < b.z;
  return a.x < b.x;
}



int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  int n, k;
  std::cin >> n >> k;
  std::vector<node> a(n + 1);
  std::vector<int> b(n + 1), bj(n + 1);

  for (int i = 1; i <= n; i++) {
    std::cin >> a[i].x >> a[i].y >> a[i].z;
    a[i].id = i;
  }

  std::sort(a.begin() + 1, a.begin() + n + 1, cmp1);

  fenw<int> f(k + 1);

  std::function<void(int, int)> cdq = [&](int l, int r) {
    if (l == r) return;
    int m = (l + r) >> 1;
    cdq(l, m);
    cdq(m + 1, r);
    sort(a.begin() + l, a.begin() + r + 1, cmp2);
    for (int i = l; i <= r; i++) {
      if (a[i].x <= m) {
        f.add(a[i].z, 1);
      } else {
        b[a[i].id] += f.cal(a[i].z);
      }
    }
    for (int i = l; i <= r; i++) {
      if (a[i].x <= m) {
        f.add(a[i].z, -1);
      }
    }
  };


  for (int i = 1; i <= n;) {
    int j = i + 1;
    while (j <= n && a[j].x == a[i].x && a[j].y == a[i].y && a[j].z == a[i].z)
      j++;
    while (i < j)
      bj[a[i].id] = a[j - 1].id, i++;
  }

  for (int i = 1; i <= n; i++) {
    a[i].x = i;
  }

  cdq(1, n);

  std::vector<int> ans(n + 1);

  for (int i = 1; i <= n; i++) {
    ans[b[bj[a[i].id]]]++;
  }

  for (int i = 0; i < n; i++) {
    printf("%d\n", ans[i]);
  }
}
