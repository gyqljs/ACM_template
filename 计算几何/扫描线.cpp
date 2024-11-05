// https://www.luogu.com.cn/problem/P5490
#include <bits/stdc++.h>

const int N = 200005;
struct line {
  int x1, x2, y;
  int tag; // 入边:+1,出边:-1
  bool operator<(const line &t) const { return y < t.y; }
} L[N];

struct tree {
  int cnt = 0, len = 0; // 区间覆盖次数和覆盖长度
} tr[N * 8];
int X[N];


void pushup(int u, int l, int r) {
  if (tr[u].cnt)
    tr[u].len = X[r + 1] - X[l];
  else
    tr[u].len = tr[2 * u].len + tr[2 * u + 1].len;
}

void change(int u, int l, int r, int tag, int x, int y) { // 区修
  if (l > y || r < x)
    return;
  if (l <= x && y <= r) {
    tr[u].cnt += tag;
    pushup(u, x, y);
    return;
  }
  int m = (x + y) >> 1;
  change(2 * u, l, r, tag, x, m);
  change(2 * u + 1, l, r, tag, m + 1, y);
  pushup(u, x, y);
}

int main() {
  std::ifstream file("text.in");
  if (file.is_open() and !file.eof())
    freopen("text.in", "r", stdin);
  int n, x1, x2, y1, y2;
  std::cin >> n;
  for (int i = 1; i <= n; i++) {
    scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
    L[i] = {x1, x2, y1, 1};
    L[n + i] = {x1, x2, y2, -1};
    X[i] = x1;
    X[n + i] = x2;
  }
  n *= 2;
  std::map<int, int> mp;
  int idx = 0;
  for (int i: X)
    mp[i] = 0;
  for (auto &it: mp)
    it.second = idx++;
  std::sort(L + 1, L + n + 1); // 扫描线排序
  std::sort(X + 1, X + n + 1); // X坐标排序
  int s = std::unique(X + 1, X + n + 1) - X - 1; // 去重
  long long ans = 0;
  for (int i = 1; i < n; i++) {
    int l = mp[L[i].x1], r = mp[L[i].x2];
    change(1, l, r - 1, L[i].tag, 1, n);
    ans += 1ll * tr[1].len * (L[i + 1].y - L[i].y);
  }
  std::cout << ans << '\n';
}