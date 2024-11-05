#include <bits/stdc++.h>

typedef long long ll;

using pii = std::pair<ll, ll>;
bool operator<=(pii a, pii b) {
  return (ll) (a.first * b.second) <= (ll) (a.second * b.first);
}

template<class T>
bool operator<=(pii a, T b) {
  return (a.first) <= (b * a.second);
}


// 由点i和点j确定的斜率
// 推荐使用分数比较避免精度问题
pii slope(int i, int j);

// 方程里面的斜率
int k(int i);

//
void monotonic_queue(int n) {
  // 初始化双端队列并添加第一个索引 (0)
  std::deque<int> q;
  q.push_back(0);

  for (int i = 1; i <= n; i++) {
    // 当前斜率小于或等于前一个斜率
    while (q.size() >= 2 && slope(i - 1, q.back()) <= slope(q.back(), q[q.size() - 2])) {
      q.pop_back();
    }
    // 将当前索引添加到双端队列
    q.push_back(i - 1);

    // 如果下一个斜率更优，从双端队列的前面移除
    while (q.size() >= 2 && slope(q[1], q[0]) <= k(i)) {
      q.pop_front();
    }

    // j 就是最优决策点
    int j = q.front();
  }
}

int calc(int i, int j);

void monotonic_stack(int n) {
  std::vector<std::vector<int>> sk(n + 1);

  for (int i = 1; i <= n; ++i) {
    while (sk[i].size() >= 2 && slope(sk[i][sk[i].size() - 2], i) >= slope(sk[i][sk[i].size() - 2], sk[i].back()))
      sk[i].pop_back();
    sk[i].push_back(i);
    while (sk[i].size() >= 2 && calc(i, sk[i].back()) <= calc(i, sk[i][sk[i].size() - 2]))
      sk[i].pop_back();
  }
}

// 斜率不单调
void solve(int n) {
  // 初始化双端队列并添加第一个索引 (0)
  std::deque<int> q;
  q.push_back(0);
  for (int i = 1; i <= n; i++) {

    while (q.size() >= 2 && slope(i - 1, q.back()) <= slope(q.back(), q[q.size() - 2])) {
      q.pop_back();
    }
    q.push_back(i - 1);

    // 在队列中进行二分搜索以找到最佳的 j
    int l = 0, r = q.size() - 1;
    int j = q[0];
    while (l <= r) {
      int m = l + (r - l) / 2;
      if (m < q.size() - 1) {
        // 这个比较可能有精度问题 如果斜率可以写成 y / x 推荐使用y <= k(i) * x
        if (slope(q[m + 1], q[m]) <= k(i)) {
          l = m + 1;
        } else {
          r = m - 1;
        }
      } else {
        break;
      }
    }

    if (l < q.size()) {
      j = q[l];
    }
    // j为最优决策点
  }
}