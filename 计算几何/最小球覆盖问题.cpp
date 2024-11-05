
#include <bits/stdc++.h>

#define int long long
#define double long double


const double eps = 1e-12;
const double pi = acos(-1);




using poi = std::pair<double, double>;
#define x first
#define y second
template<typename T>
T squ(T x) {
  return x * x;
}
poi operator+(poi a, poi b) {
  return poi(a.x + b.x, a.y + b.y);
}
poi operator/(poi a, double k) {
  return poi(a.x / k, a.y / k);
}
double dis(poi a, poi b) {
  return sqrt(squ(a.x - b.x) + squ(a.y - b.y));
}
using poi_set = std::vector<poi>;
using cir = std::pair<double, poi>;
#define r first
#define o second

cir get_cir(poi a, poi b) {
  return cir(dis(a, b) / 2, (a + b) / 2);
}

cir get_cir(poi &a, poi &b, poi &c) {
  double a1 = b.x - a.x, a2 = c.x - a.x, b1 = b.y - a.y, b2 = c.y - a.y;
  double c1 = (squ(b.x) - squ(a.x) + squ(b.y) - squ(a.y));
  double c2 = (squ(c.x) - squ(a.x) + squ(c.y) - squ(a.y));
  poi o = {(b2 * c1 - b1 * c2) / (b2 * a1 * 2 - b1 * a2 * 2), (a2 * c1 - a1 * c2) / (a2 * b1 * 2 - a1 * b2 * 2)};
  return cir(dis(o, a), o);
}

// 点与圆关系
// 0 圆外 | 1 圆上 | 2 圆内
int poi_cir(poi &a, cir &b) {
  double d = dis(a, b.o);
  if (fabs(d - b.r) < eps)
    return 1;
  if (d > b.r + eps)
    return 0;
  return 2;
}




int32_t main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::random_device rd;
  std::mt19937 g(rd());
  std::ifstream file("text.in");
  if (file.peek() != std::ifstream::traits_type::eof())
    freopen("text.in", "r", stdin);

  int n;
  std::cin >> n;
  poi_set v(n + 1);
  for (int i = 1; i <= n; i++)
    std::cin >> v[i].x >> v[i].y;
  std::shuffle(v.begin() + 1, v.end(), g);
  cir c;
  c.r = 0;
  c.o = v[1];
  for (int i = 2; i <= n; i++) {
    if (poi_cir(v[i], c) == 0) {
      c.r = 0;
      c.o = v[i];
      for (int j = 1; j < i; j++) {
        if (poi_cir(v[j], c) == 0) {
          c = get_cir(v[i], v[j]);
          for (int k = 1; k < j; k++) {
            if (poi_cir(v[k], c) == 0)
              c = get_cir(v[i], v[j], v[k]);
          }
        }
      }
    }
  }


  printf("%.9Lf\n", c.r);
  printf("%.9Lf %.9Lf", c.o.x, c.o.y);
}