#include <cstdio>
#include <string>
#include <vector>

using namespace std;

int main() {
    int n;
    char buffer[200005];
    if (scanf("%d%s", &n, buffer) != 2) return 0;
    string s = buffer;

    // 将序列两两分组:
    // (奇数位, 偶数位)
    // "GG" 或 "HH": 翻转前后内部都不变, 无影响, 直接忽略
    // "GH": 初始时 G 位于奇数位 (不良状态), 翻转后变成 "HG"
    // "HG": 初始时 G 位于偶数位 (优良状态), 翻转后变成 "GH"
    vector<char> simplified;
    for (int i = 0; i < n; i += 2) {
        if (s[i] == 'G' && s[i + 1] == 'H') {
            simplified.push_back('G');  // 表示当前为未满足状态
        } else if (s[i] == 'H' && s[i + 1] == 'G') {
            simplified.push_back('H');  // 表示当前为已满足状态
        }
    }

    // 每次前缀翻转偶数长度, 相当于在 simplified 序列中将前缀倒序并将每个元素互换状态 (G <-> H)
    // 逆向贪心: 从后往前扫描, 如果当前末尾处于 'G' (未满足), 必须进行一次翻转!
    int reversals = 0;
    for (int i = (int)simplified.size() - 1; i >= 0; --i) {
        // 当前元素在经过 reversals 次全局前缀翻转后, 状态是否依然为 'G'
        char currentStatus = simplified[i];
        if (reversals % 2 == 1) {
            currentStatus = (currentStatus == 'G' ? 'H' : 'G');
        }
        if (currentStatus == 'G') {
            ++reversals;
        }
    }

    printf("%d\n", reversals);
    return 0;
}
