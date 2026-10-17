#include <cstdio>
#include <set>
using namespace std;

const int MAXN = 300005;

int p[MAXN];
char s[MAXN];

// 只考虑从下标 from 开始的后缀，求「与字符串的某一段同向」的最快子序列：
// 用经典的 LIS/LDS 贪心（维护候选末尾值的集合）在线加入元素。
// 集合中存的是各个长度的最优末尾值；每成功扩展一次，匹配的字符数就加一。
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &p[i]);
    scanf("%s", s);
    int slen = 0;
    while (s[slen] != '\0') slen++;

    int matched = 0;   // 目前已匹配的字符串前缀长度
    int idx = 1;       // 下一个还没读入的排列元素下标
    int last = p[0];   // 当前最快子序列的末尾取值（也就是刚读入的那个元素）
    while (idx < n && matched < slen) {
        char dir = s[matched];
        // 上一段结束时留下的末尾值本身就是长度为 1 的候选末尾
        set<int> tails;
        tails.insert(last);
        while (idx < n && matched < slen && s[matched] == dir) {
            last = p[idx++];
            if (dir == 'U') {
                // 需要 a_{j-1} < a_j，找最小的比 last 大的末尾值替换（贪心保持最优）
                set<int>::iterator it = tails.upper_bound(last);
                if (it == tails.end()) {
                    matched++;         // 无法替换，说明子序列变长了
                } else {
                    tails.erase(it);   // 用更小的末尾值替换，为后续留出空间
                }
            } else {
                // 需要 a_{j-1} > a_j，找最大的比 last 小的末尾值替换
                set<int>::iterator it = tails.lower_bound(last);
                if (it == tails.begin()) {
                    matched++;
                } else {
                    --it;
                    tails.erase(it);
                }
            }
            tails.insert(last);
        }
    }
    printf("%d\n", matched);
    return 0;
}
