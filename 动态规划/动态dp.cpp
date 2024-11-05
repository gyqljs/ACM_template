#include <bits/stdc++.h>


const int N = 1000005;

const int inf = 0x3f3f3f3f;

int idx = 0;
struct node {
  int t, n;
} gh[N << 1];
int head[N];

void add(int a, int b) { gh[++idx].t = b, gh[idx].n = head[a], head[a] = idx; }

int w[N], root[N], nod;
int fa[N], sz[N], son[N], dfn[N], id[N], top[N], bot[N], f[N][2], tot;

using matrix = std::array<std::array<int, 2>, 2>;

matrix operator*(matrix a, matrix b) {
  matrix t;
  t[0][0] = std::max(a[0][0] + b[0][0], a[0][1] + b[1][0]);
  t[1][0] = std::max(a[1][0] + b[0][0], a[1][1] + b[1][0]);
  t[0][1] = std::max(a[0][0] + b[0][1], a[0][1] + b[1][1]);
  t[1][1] = std::max(a[1][0] + b[0][1], a[1][1] + b[1][1]);
  return t;
}

matrix mt[N];

void dfs(int x) {
  sz[x] = 1;
  f[x][1] = w[x];
  for (int i = head[x]; i != -1; i = gh[i].n) {
    int y = gh[i].t;
    if (y == fa[x]) continue;
    fa[y] = x;
    dfs(y);
    sz[x] += sz[y];
    if (sz[y] > sz[son[x]]) son[x] = y;
    f[x][0] += std::max(f[y][1], f[y][0]);
    f[x][1] += f[y][0];
  }
}

void dfs(int x, int tp) {
  dfn[x] = ++tot;
  id[tot] = x;
  top[x] = tp;
  bot[tp] = tot;
  if (son[x]) dfs(son[x], tp);
  mt[x][1][0] = w[x];
  mt[x][1][1] = -inf;
  for (int i = head[x]; i != -1; i = gh[i].n) {
    int y = gh[i].t;
    if (y == fa[x] || y == son[x]) continue;
    dfs(y, y);
    mt[x][0][0] += std::max(f[y][0], f[y][1]);
    mt[x][0][1] = mt[x][0][0];
    mt[x][1][0] += f[y][0];
  }
}

class segf {
public:
  struct node {
    int l, r;
    matrix v;
  } tr[4 * N];

  void pushup(int u) {
    tr[u].v = tr[tr[u].l].v * tr[tr[u].r].v;
  }

  void build(int &u, int l, int r) { // 建线段树
    u = ++nod;
    if (l == r) {
      tr[u].v = mt[id[l]];
      return;
    }
    int m = (l + r) >> 1;
    build(tr[u].l, l, m);
    build(tr[u].r, m + 1, r);
    pushup(u);
  }

  void change(int u, int l, int r, int p) { // 点修
    if (l == r) {
      tr[u].v = mt[id[l]];
      return;
    }
    int m = (l + r) >> 1;
    if (p <= m)
      change(tr[u].l, l, m, p);
    else
      change(tr[u].r, m + 1, r, p);
    pushup(u);
  }

  matrix query(int u, int l, int r, int x, int y) {
    if (x == l and r == y) return tr[u].v;
    int m = (l + r) >> 1;
    if (y <= m) return query(tr[u].l, l, m, x, y);
    if (x > m) return query(tr[u].r, m + 1, r, x, y);
    return query(tr[u].l, l, m, x, m) * query(tr[u].r, m + 1, r, m + 1, y);
  }

  void update(int u, int v) { // 修改点权
    mt[u][1][0] += v - w[u];
    w[u] = v;
    while (u) {
      matrix a = tr[root[top[u]]].v;
      change(root[top[u]], dfn[top[u]], bot[top[u]], dfn[u]);
      matrix b = tr[root[top[u]]].v;
      u = fa[top[u]]; // 上跳
      mt[u][0][0] += std::max(b[0][0], b[1][0]) - std::max(a[0][0], a[1][0]);
      mt[u][0][1] = mt[u][0][0];
      mt[u][1][0] += b[0][0] - a[0][0];
    }
  }
};

template<class T = int>
inline T read(int64_t m = INT64_MAX) {
  T x = 0, f = 1;
  char ch = getchar();
  while (ch > '9' || ch < '0') {
    if (ch == '-') f = -1;
    ch = getchar();
  }
  while (ch >= '0' && ch <= '9') {
    x *= 10;
    x += (ch - '0');
    x %= m;
    ch = getchar();
  }
  return x * f % m;
}


int main() {
  std::ifstream file("text.in");
  if (file.is_open() and !file.eof())
    freopen("text.in", "r", stdin);
  int n, m;
  memset(head, -1, sizeof(head));
  n = read(), m = read();
  int x, y;
  for (int i = 1; i <= n; i++) w[i] = read();
  for (int i = 1; i < n; i++) {
    x = read(), y = read();
    add(x, y);
    add(y, x);
  }
  segf s;
  dfs(1);
  dfs(1, 1);
  for (int i = 1; i <= n; i++)
    if (top[i] == i) s.build(root[i], dfn[i], bot[i]);
  for (int i = 1; i <= m; i++) {
    x = read(), y = read();
    s.update(x, y);
    matrix ans = s.tr[root[1]].v;
    printf("%d\n", std::max(ans[0][0], ans[1][0]));
  }
}