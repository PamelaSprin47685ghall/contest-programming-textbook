#include <cstdio>
#include <vector>

// 检查日志是否自洽, 若自洽则算出最少/最多可能的驯服次数.
// 成功返回 true 并写出 min 和 max; 矛盾返回 false.
bool Compute(int& min, int& max) {
    int n;
    scanf("%d", &n);
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) scanf("%d", &a[i]);
    // 第 0 天必须发生一次驯服.
    if (0 < a[0]) return false;
    a[0] = 0;
    // 从最后一天往前推: 若某天驯服, 之后每天的数字依次递减.
    // current 表示当前正在倒数的"距上次驯服的天数".
    int current = -1;
    for (int i = n - 1; i >= 0; --i) {
        if (current != -1) --current;
        if (current == -1) {
            // 前一段已结束, 从这一天的数字重新开始倒数.
            current = a[i];
            continue;
        }
        if (a[i] == -1) {
            // 未知, 用推导出的数字补上.
            a[i] = current;
            continue;
        }
        if (a[i] != current) return false;
    }
    // 0 表示确定发生过驯服的日子, -1 表示未知(可驯服可不驯服).
    int required = 0, optional = 0;
    for (int value : a) {
        required += (value == 0);
        optional += (value == -1);
    }
    min = required;
    max = required + optional;
    return true;
}

int main() {
    int min, max;
    if (Compute(min, max)) {
        printf("%d %d\n", min, max);
    } else {
        printf("-1\n");
    }
    return 0;
}