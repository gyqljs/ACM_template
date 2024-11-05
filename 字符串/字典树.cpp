#include <bits/stdc++.h>

// 字典树
class trie {
private:
  // ch[p][j]表示从p沿着字母j走下的子节点
  // 0～j存储了26个英文字母
  int n = 0;
  struct node {
    int cr[26] = {};
    // sz存储以节点这个节点为结尾的单词的插入次数
    int sz = 0;
    int &operator[](int x) {
      return cr[x];
    }
  };
  std::vector<int> flag;
  std::vector<node> ch;

public:
  explicit trie(): ch(1) {}

  void insert(std::string &s) {
    int p = 0;
    for (auto i: s) {
      int j = i - 'a';
      if (!ch[p][j]) {
        ch[p][j] = ch.size();
        ch.push_back(node());
      }
      p = ch[p][j];
      ch[p].sz++;
    }
  }

  int query(std::string &s) {
    int p = 0;
    for (auto i: s) {
      int j = i - 'a';
      // 一直往下面走
      p = ch[p][j];
      if (!p)
        return 0;
    }
    return ch[p].sz;
  }
};
