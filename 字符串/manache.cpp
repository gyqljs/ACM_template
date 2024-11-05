#include <bits/stdc++.h>

class manache {
private:
  std::deque<int> d;

public:
  explicit manache(std::string &s) {
    std::string t;
    for (int i = 0; i < s.size(); i++) {
      t += std::string("#") + s[i];
    }
    t += '#';
    int num = t.size();
    t = "$" + t + "^";
    d.resize(num + 2);
    int l = 0, r = 0;
    for (int i = 0; i < num; i++) {
      d[i] = std::max((int) 0, std::min(r - i, d[l + (r - i)]));
      while (t[i - d[i]] == t[i + d[i]]) {
        d[i]++;
      }
      if (i + d[i] > r) {
        l = i - d[i], r = i + d[i];
      }
    }
    // 输出回文串长度减去1
    // for (int i = 0; i < num; i++) {
    //     std::cout << d[i] - 1 << " ";
    // }
  }


  // 检查l～r区间内的字符串是不是会文串
  bool check(int l, int r) {
    if (d[l + r] == r - l + 1)
      return true;
    else
      return false;
  }
};