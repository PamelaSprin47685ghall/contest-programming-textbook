#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

int main() {
  int n, q;
  scanf("%d%d", &n, &q);
  vector<string> grid(n);
  char buffer[2005];
  for(auto& x: grid) { scanf("%s", buffer); x = buffer; }
  vector<vector<int>> canonical(n/2);
  for(auto& x: canonical) x.resize(n/2);
  int ans = 0;
  auto apply = [&](int x, int y, int scale) -> void {
    if(grid[x][y] == '.') return;
    x = min(x, n-1-x);
    y = min(y, n-1-y);
    ans -= min(canonical[x][y], 4-canonical[x][y]);
    canonical[x][y] += scale;
    ans += min(canonical[x][y], 4-canonical[x][y]);
  };
  for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) apply(i, j, 1);
  printf("%d\n", ans);
  while(q--) {
    int x, y;
    scanf("%d%d", &x, &y);
    x--; y--;
    apply(x, y, -1);
    grid[x][y] = '#' + '.' - grid[x][y];
    apply(x, y, 1);
    printf("%d\n", ans);
  }
}
