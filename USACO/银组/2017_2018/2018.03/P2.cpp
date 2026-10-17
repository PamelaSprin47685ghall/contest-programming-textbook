#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100005;

int n;
int willing[MAXN];  // 每头牛最多愿意排在多少头牛后面

// 从大到小排序: 容忍度高的牛先到, 才能把挑剔的牛挡在队伍外
bool cmpDesc(int a, int b) { return a > b; }

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &willing[i]);

    sort(willing, willing + n, cmpDesc);

    // 让 w 大的牛先进队: 队列里已有 inLine 头牛时, 只有当 inLine <= w 她才肯进队.
    // 一旦当前这头牛进不来, 后面所有牛的 w 都不比她大, 更进不来, 队伍规模就此定型.
    int inLine = 0;
    for (int i = 0; i < n; i++) {
        if (willing[i] < inLine) break;
        inLine++;
    }

    printf("%d\n", inLine);
    return 0;
}
