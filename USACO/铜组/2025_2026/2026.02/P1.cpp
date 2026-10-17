#include <cstdio>
#include <string>

using namespace std;

char buffer[200005]; // N <= 2e5

int main() {
  int t, mode;
  scanf("%d %d", &t, &mode);
  while(t--) {
    int n;
    scanf("%d", &n);
    scanf("%s", buffer);
    string s = buffer;
    printf("YES\n");
    if(mode == 0) continue;
    for(int i = 0; i+1 < n; i++) {
      if(s[i] == s[i+1]) putchar('M');
      else putchar('O');
    }
    printf("%c\n", s.back());
  }
  return 0;
}
