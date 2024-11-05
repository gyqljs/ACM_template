#include <bits/stdc++.h>

// 数字进制的转换
std::string cb(std::string num, int b1, int b2) {
  int d = 0;
  std::string r = "";
  for (char digit: num) {
    d = d * b1 + (isdigit(digit) ? digit - '0' : digit - 'A' + 10);
  }
  while (d) {
    int re = d % b2;
    r += (std::to_string(re > 9 ? re - 10 + 'A' : re + '0'));
    d /= b2;
  }
  reverse(r.begin(), r.end());
  return r;
}