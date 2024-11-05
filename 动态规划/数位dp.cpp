// 数位dp基础模板

#include <bits/stdc++.h>

std::string s;
std::vector<int> f;

// flag表示当前数字是否严格小于s[pos]
// 不考虑前导0
int dfs1(int pos, bool flag) {
  if (pos == s.size())
    return 1;
  if (!flag and f[pos] != -1)
    return f[pos];
  int l = 0, r = flag ? s[pos] - '0' : 9;
  int ret = 0;
  for (int i = l; i <= r; i++) {

    ret += dfs1(pos + 1, flag and i == r);
  }
  if (!flag) f[pos] = ret;
  return ret;
}

// 考虑前导0
// lead为ture表示之前所有位全是0
int dfs2(int pos = 0, bool flag = 1, int lead = 1) {
  if (pos == s.size())
    return !lead;
  if (!flag and !lead and f[pos] != -1)
    return f[pos];
  int ret = 0;
  if (lead)
    ret = dfs2(pos + 1, 0, 1);
  int l = 0, r = flag ? s[pos] - '0' : 9;
  for (int i = std::max(l, lead); i <= r; i++) {

    ret += dfs2(pos + 1, flag and i == r, 0);
  }
  if (!flag and !lead)
    f[pos] = ret;
  return ret;
}

// int solve(int num) {
//   s = std::to_string(num);
// }