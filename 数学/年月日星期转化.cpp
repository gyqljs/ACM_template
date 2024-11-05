#include <bits/stdc++.h>

int qpow(int b, int e, int m = INT32_MAX) {
  int r = 1;
  while (e) {
    if (e & 1)
      r = r * b % m;
    b = b * b % m;
    e /= 2;
  }
  return r % m;
}


// 生成0 10 11…………的整数
void gen(int n, std::vector<int> &sq) {
  for (int i = 1; i <= n; ++i) {
    int num = 0;
    int t = i;
    int b = 1;
    while (t > 0) {
      int l = t % 2;
      num += l * b;
      t /= 2;
      b *= 10;
    }
    sq.push_back(num);
  }
}

// 判定一个数字是不是完全n次方数
bool checkpow(int a, int k) {
  int x = pow(a, 1.0 / k) + 1e-5;
  return qpow(x, k) == a;
}


// 年月日和星期的转换
int getdata(int y, int m, int d) {
  if (m == 1 || m == 2) {
    y--;
    m += 12;
  }
  return (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7 + 1;
}