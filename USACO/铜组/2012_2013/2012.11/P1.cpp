#include <cstdio>
#include <string>

using namespace std;

int main() {
    char buffer[50001];
    scanf("%s", buffer);
    string s = buffer;

    // left: 已经出现的 "((" 对数; 每遇到一个 "))" 都能与之前的 "((" 组成一对.
    int left = 0;
    int answer = 0;
    for (int i = 1; i < (int)s.size(); ++i) {
        if (s[i - 1] == '(' && s[i] == '(') ++left;
        if (s[i - 1] == ')' && s[i] == ')') answer += left;
    }
    printf("%d\n", answer);
    return 0;
}
