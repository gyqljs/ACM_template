#include <bits/stdc++.h>



void floyd(int n, std::vector<std::bitset<301>> &a) {
  for (int j = 1; j <= n; j++)
    for (int i = 1; i <= n; i++)
      if (a[i][j])
        a[i] |= a[j];
}

int u;

int main() {
  int n;
  std::cin >> n;
  std::vector<std::bitset<301>> a(301);
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      std::cin >> u;
      a[i][j] = u;
    }
  }
  floyd(n, a);
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++)
      putchar(a[i][j] + '0'), putchar(' ');
    putchar('\n');
  }
}