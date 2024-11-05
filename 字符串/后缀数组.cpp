#include <bits/stdc++.h>


// h[i] = lcp(sa[i], sa[i - 1])
struct SA {
  int n;
  std::vector<int> sa, rk, h;

  explicit SA(std::string &s): n(s.size()), sa(n), rk(n), h(n - 1) {
    std::iota(sa.begin(), sa.end(), 0);
    std::sort(sa.begin(), sa.end(), [&](int a, int b) { return s[a] < s[b]; });
    rk[sa[0]] = 0;
    for (int i = 1; i < n; i++)
      rk[sa[i]] = rk[sa[i - 1]] + (s[sa[i]] != s[sa[i - 1]]);
    int k = 1;
    std::vector<int> t, cnt(n);
    t.reserve(n);
    while (rk[sa[n - 1]] < n - 1) {
      t.clear();
      for (int i = 0; i < k; i++)
        t.push_back(n - k + i);

      for (auto i: sa)
        if (i >= k)
          t.push_back(i - k);


      std::fill(cnt.begin(), cnt.end(), 0);
      for (int i = 0; i < n; i++)
        cnt[rk[i]]++;

      for (int i = 1; i < n; i++)
        cnt[i] += cnt[i - 1];

      for (int i = n - 1; i >= 0; i--)
        sa[--cnt[rk[t[i]]]] = t[i];

      std::swap(rk, t);
      rk[sa[0]] = 0;
      for (int i = 1; i < n; i++)
        rk[sa[i]] = rk[sa[i - 1]] + (t[sa[i - 1]] < t[sa[i]] || sa[i - 1] + k == n || t[sa[i - 1] + k] < t[sa[i] + k]);
      k *= 2;
    }

    //
    for (int i = 0, j = 0; i < n; i++) {
      if (rk[i] == 0)
        j = 0;
      else {
        j -= j > 0;
        while (i + j < n && sa[rk[i] - 1] + j < n && s[i + j] == s[sa[rk[i] - 1] + j])
          j++;
        h[rk[i] - 1] = j;
      }
    }
  }
};