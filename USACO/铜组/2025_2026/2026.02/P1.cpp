#include <iostream>

using namespace std;

int main() {
  int t, mode;
  cin >> t >> mode;
  while(t--) {
    int n;
    string s;
    cin >> n >> s;
    cout << "YES\n";
    if(mode == 0) continue;
    for(int i = 0; i+1 < n; i++) {
      if(s[i] == s[i+1]) cout << 'M';
      else cout << 'O';
    }
    cout << s.back() << "\n";
  }
}