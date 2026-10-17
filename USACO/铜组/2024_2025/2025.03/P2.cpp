#include <cstdio>
#include <vector>

using namespace std;

int main() {
  int t;
  scanf("%d", &t);
  for(int cn = 1; cn <= t; cn++) {
    int n;
    scanf("%d", &n);
    vector<int> f(n+1);
    for(int i = 0; i < n; i++) {
      int x;
      scanf("%d", &x);
      f[x]++;
    }
    int ret = 0;
    bool seen = false;
    for(int i = n; i > 0; i--) {
      if(f[i] == 0) continue;
      if(!seen) ret++;
      else if(f[i] >= 2) ret += 2;
      seen = true;
    }
    printf("%d\n", ret);
  }
}
