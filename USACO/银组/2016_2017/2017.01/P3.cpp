#include <cstdio>
#include <cstring>

using namespace std;

const int MAXLEN = 30;

char s[MAXLEN + 5];
int slen;

// 求无限长串中下标 idx 处的字符(下标从 0 开始)
char solve(long long idx) {
    if (idx < slen) return s[idx];      // 落在初始串里, 直接返回

    // 当前层由长度 len 的串翻倍得到, 找到最小的 len 使 idx 落在后半段 [len, 2*len)
    long long len = slen;
    while (2 * len <= idx) len *= 2;

    // 后半段是原串的右旋: 首字符来自原串末字符, 其余字符整体后移一位
    if (idx == len) return solve(len - 1);
    return solve(idx - len - 1);
}

int main() {
    long long n;
    scanf("%s %lld", s, &n);
    slen = strlen(s);
    printf("%c\n", solve(n - 1));
    return 0;
}
