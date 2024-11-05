#include <bits/stdc++.h>


class lct {
private:
  struct node { // splay的信息
    // 左右儿子 父亲  值  异或和
    int ch[2], fa, v, sum;
    int tag; // 翻转懒标记
  };

  std::vector<node> tr;

  int &fa(int x) { return tr[x].fa; }

  int &lc(int x) { return tr[x].ch[0]; }

  int &rc(int x) { return tr[x].ch[1]; }

  bool noroot(int x) { return lc(fa(x)) == x || rc(fa(x)) == x; }

  // 更新x的信息
  void update(int x) {
    tr[x].sum = tr[lc(x)].sum ^ tr[x].v ^ tr[rc(x)].sum;
  }

  void pushdown(int x) {
    if (tr[x].tag) {
      std::swap(lc(x), rc(x));
      tr[lc(x)].tag ^= 1;
      tr[rc(x)].tag ^= 1;
      tr[x].tag = 0;
    }
  }

  void pushall(int x) { // 递归下传
    if (noroot(x)) pushall(fa(x));
    pushdown(x);
  }

  void rotate(int x) {
    int y = fa(x), z = fa(y), k = rc(y) == x; // y的右儿是x
    if (noroot(y))
      tr[z].ch[rc(z) == y] = x;
    fa(x) = z; // z的儿是x,x的父是z
    tr[y].ch[k] = tr[x].ch[k ^ 1];
    fa(tr[x].ch[k ^ 1]) = y; // y的儿是x的异儿,x的异儿的父是y
    tr[x].ch[k ^ 1] = y;
    fa(y) = x; // x的异儿是y,y的父是x
    update(y);
    update(x); // 自底向上pushup
  }

  // x伸展到根
  void splay(int x) {
    pushall(x); // 递归下传
    while (noroot(x)) { // 折线转xx,直线转yx
      int y = fa(x), z = fa(y);
      if (noroot(y)) (rc(y) == x) ^ (rc(z) == y) ? rotate(x) : rotate(y);
      rotate(x);
    }
  }

  // 打通x到树根的路 就是将此路径上面的所有变变成实边
  void access(int x) {
    for (int y = 0; x;) {
      splay(x); // x转到当前splay的根
      rc(x) = y; // x的右儿指向下面splay的根
      update(x); // 更新x的sum
      y = x, x = fa(x); // 存x,x爬到上面的splay
    }
  }

  // 换根
  void makeroot(int x) {
    access(x); // 通路
    splay(x); // 伸展
    tr[x].tag ^= 1; // 翻转懒标记
  }

  // 分离x到y的路径, 把y转到根
  void split(int x, int y) {
    makeroot(x); // x换根
    access(y); // y通路
    splay(y); // y伸展
  }

public:
  void output(int x, int y) { // 输出
    split(x, y); // 分离
    printf("%d\n", tr[y].sum);
  }

  int findroot(int x) { // 找根
    access(x);
    splay(x);
    while (lc(x)) pushdown(x), x = lc(x);
    splay(x); // 防止卡链
    return x;
  }

  void link(int x, int y) { // 连边
    makeroot(x);
    if (findroot(y) != x)
      fa(x) = y;
  }

  void cut(int x, int y) { // 断边
    makeroot(x);
    if (findroot(y) == x && fa(y) == x && !lc(y))
      fa(y) = 0, update(x);
  }

  void change(int x, int y) {
    splay(x);
    tr[x].v = y;
    update(x);
  }

  int &operator[](int id) {
    return tr[id].v;
  }

  explicit lct(int n): tr(n + 2) {}
};