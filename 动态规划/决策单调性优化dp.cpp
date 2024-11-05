#include <bits/stdc++.h>

const int N = 10005;

const int inf = 1e9;
int n, dp[N];
int cost(int j, int i); // dp转移方程

// ref(1, n, 1, n);
std::function<void(int, int, int, int)> ref = [](int l, int r, int optL, int optR) {
  if (l > r) return;
  int m = (l + r) / 2;
  // 最优的决策值以及他的位置
  std::pair<int, int> best = {inf, -1};
  for (int k = optL; k <= std::min(m, optR); k++) {
    int cur = dp[k] + cost(k, m);
    if (cur < best.first) {
      best = {cur, k};
    }
  }
  dp[m] = best.first;
  int opt = best.second;
  ref(l, m - 1, optL, opt);
  ref(m + 1, r, opt, optR);
};
// 如果无限制则交换一遍a和dp倒序输出
// ref(1, n, 1, n);
// for (int i = 1; i <= n / 2; i++)
//     std::swap(a[i], a[n - i + 1]), std::swap(dp[i], dp[n - i + 1]);
// ref(1, n, 1, n);
// for (int i = n; i >= 1; i--) {
//     std::cout << (int) ceil(dp[i]) << '\n';
// }


int val(int j, int i);


auto solve = []() {
  // 双端队列中的每个元素存储 {lt, rt, pt}
  std::deque<std::array<int, 3>> d;

  // 初始化为 lt = 1, rt = n, pt = 0    d
  d.push_back({1, n, 0});

  for (int i = 1; i <= n; ++i) {
    // 使用前面元素的 pt 更新答案
    dp[i] = val(d[0][2], i);

    if (i == d[0][1]) {
      // 如果当前索引到达右边界，则删除前面的索引
      d.pop_front();
    } else {
      // 否则，增加前面元素的左边界
      d[0][0]++;
    }
    // 根据val函数以升序维护双端队列
    while (!d.empty() && val(d.back()[2], d.back()[0]) >= val(i, d.back()[0])) {
      d.pop_back();
    }

    if (d.empty()) {
      // 如果双端队列为空，则压入新范围{i+1, n, i}
      d.push_back({i + 1, n, i});
    } else {
      int k;
      // 将最后一个元素的 pt 的 val 与新元素的 pt 进行比较
      if (val(d.back()[2], d.back()[1]) <= val(i, d.back()[1])) {
        k = d.back()[1] + 1;
      } else {
        // 二分查找寻找分割点
        int l = d.back()[0];
        int r = d.back()[1];
        while (l < r) {
          int m = l + (r - l) / 2;
          if (val(d.back()[2], m) >= val(i, m))
            r = m;
          else
            l = m + 1;
        }
        k = r;
      }

      if (k <= n) {
        // 更新最后一个元素的右边界
        d.back()[1] = k - 1;
        // 压入新的范围{k, n, i}
        d.push_back({k, n, i});
      }
    }
  }
};