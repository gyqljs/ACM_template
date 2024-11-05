#include <bits/stdc++.h>

int get_min(std::vector<int> &s) {
  int n = s.size();
  s.resize(2 * s.size());
  for (int i = 0; i < n; i++)
    s[n + i] = s[i];
  int i = 0, j = 1;
  while (i < n and j < n) {
    int k = 0;
    while (k < n - 1 and s[i + k] == s[j + k])
      k++;
    s[i + k] > s[j + k] ? i = i + k + 1 : j = j + k + 1;
    if (i == j)
      j++;
  }
  return std::min(i, j);
}