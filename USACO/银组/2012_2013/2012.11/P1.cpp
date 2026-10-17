#include <cstdio>
#include <cstring>

using namespace std;

const int MAXN = 100005;

char s[MAXN];

int main() {
    scanf("%s", s);
    int n = strlen(s);

    int depth = 0;  // 到目前为止还没被配对的左括号数
    int answer = 0;
    for (int i = 0; i < n; ++i) {
        if (s[i] == '(') {
            ++depth;
        } else {
            --depth;
        }
        if (depth < 0) {
            // 到这个前缀为止右括号比左括号多了, 后面的字符救不回这个前缀,
            // 必须在当前位置 (最靠后的那个右括号) 翻转一次; 翻转后它变成左括号,
            // 深度从 -1 变成 1, 所以加 2 而不是加 1
            ++answer;
            depth += 2;
        }
    }

    // 剩下的 depth 个左括号还没有配对. 每把一个靠后的左括号翻成右括号,
    // 深度就减 2, 所以还要翻 depth/2 个; 串长为偶数, depth 必为偶数
    answer += depth / 2;

    printf("%d\n", answer);
    return 0;
}
