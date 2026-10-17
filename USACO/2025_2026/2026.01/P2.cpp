#include <bits/stdc++.h>
using namespace std;
int main() {
  ios::sync_with_stdio(0); cin.tie(0);
  int t, x; cin >> t >> x;
  while (t--) {
    int n; cin >> n;
    string s; cin >> s;
    if (n & 1) {
      cout << -1 << endl;
      continue;
    }
    vector<int> ans(n * 3, 1);
    for (int i = 0; i < n / 2; i++) {
      string l = s.substr(i * 3, 3);
      string r = s.substr((i + n / 2) * 3, 3);
      if (l != r) {
        if (l.substr(0, 2) == r.substr(1, 2)) {
          ans[i * 3 + 2] = 2;
          ans[(i + n / 2) * 3] = 2;
        } else if (l.substr(1, 2) == r.substr(0, 2)) {
          ans[i * 3] = 2;
          ans[(i + n / 2) * 3 + 2] = 2;
        }  
      }
    }
    cout << *max_element(ans.begin(), ans.end()) << endl;
    for (int i = 0; i < n * 3; i++) {
      cout << ans[i] << " \n"[i == 3 * n - 1];
    }
  }
}