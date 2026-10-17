#include <array>
#include <iostream>
#include <vector>
 
int main() {
  int n, q;
  std::cin >> n >> q;
  std::vector<std::string> grid(n);
  for(auto& x: grid) std::cin >> x;
  std::vector<std::vector<int>> canonical(n/2);
  for(auto& x: canonical) x.resize(n/2);
  int ans = 0;
  auto apply = [&](int x, int y, int scale) -> void {
    if(grid[x][y] == '.') return;
    x = std::min(x, n-1-x);
    y = std::min(y, n-1-y);
    ans -= std::min(canonical[x][y], 4-canonical[x][y]);
    canonical[x][y] += scale;
    ans += std::min(canonical[x][y], 4-canonical[x][y]);
  };
  for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) apply(i, j, 1);
  std::cout << ans << "\n";
  while(q--) {
    int x, y;
    std::cin >> x >> y;
    x--; y--;
    apply(x, y, -1);
    grid[x][y] = '#' + '.' - grid[x][y];
    apply(x, y, 1);
    std::cout << ans << "\n";
  }
}