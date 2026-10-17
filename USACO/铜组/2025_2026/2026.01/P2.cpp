#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

char buffer[300005]; // 3N <= 3e5

int main() {
  int t, x;
  scanf("%d %d", &t, &x);
  while (t--) {
    int n;
    scanf("%d", &n);
    scanf("%s", buffer);
    string s = buffer;
    if (n & 1) {
      printf("-1\n");
      continue;
    }
    vector<int> ans(n * 3, 1);
    for (int i = 0; i < n / 2; i++) {
      string l = s.substr(i * 3, 3);
      string r = s.substr((i + n / 2) * 3, 3);
      if (l == r) continue;
      // 三字母循环串的最长公共片段至少有两个字母。公共片段进第 1 次删除，剩下的字母进第 2 次。
      if (l.substr(0, 2) == r.substr(0, 2)) {
        ans[i * 3 + 2] = 2;
        ans[(i + n / 2) * 3 + 2] = 2;
      } else if (l.substr(0, 2) == r.substr(1, 2)) {
        ans[i * 3 + 2] = 2;
        ans[(i + n / 2) * 3] = 2;
      } else if (l.substr(1, 2) == r.substr(0, 2)) {
        ans[i * 3] = 2;
        ans[(i + n / 2) * 3 + 2] = 2;
      } else {
        ans[i * 3] = 2;
        ans[(i + n / 2) * 3] = 2;
      }
    }
    printf("%d\n", *max_element(ans.begin(), ans.end()));
    for (int i = 0; i < n * 3; i++) {
      printf("%d%c", ans[i], i == 3 * n - 1 ? '\n' : ' ');
    }
  }
  return 0;
}
