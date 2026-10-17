#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 20;      // 灯与开关个数的上限
const long long MOD = 1000000007;

int n;

// 把开关串循环右移一位：新的第 i 位来自原来的第 i-1 位，第 0 位来自最高位
int cycRight(int x) {
    int top = (x >> (n - 1)) & 1;
    return ((x << 1) & ((1 << n) - 1)) | top;
}

// 字符串的第 i 个字符对应二进制的第 i 位（低位在前）
int strToMask(const char *s) {
    int res = 0;
    for (int i = 0; i < n; ++i) {
        if (s[i] == '1') {
            res |= 1 << i;
        }
    }
    return res;
}

int main() {
    int t;
    scanf("%d%d", &t, &n);
    int full = 1 << n;

    // rep[m]：m 所在循环移位等价类的最小代表。
    // 因为规则在循环移位下对称，等价类里的状态可以只看代表。
    vector<int> rep(full, -1);
    for (int i = 0; i < full; ++i) {
        if (rep[i] == -1) {
            int j = i;
            while (rep[j] == -1) {   // 沿循环移位走一圈，整类都记成 i
                rep[j] = i;
                j = cycRight(j);
            }
        }
    }

    // can[i][rep[m]] 表示：初始灯全灭、开关全不激活，恰用 i 步能把灯变成 m
    // （同类旋转状态等价）。vector<bool> 按位存储，3n*2^n 位只占几 MB。
    vector<vector<bool>> can(3 * n + 1, vector<bool>(full, false));
    can[0][rep[0]] = true;
    int pad = 0;   // 第一步拨动的那个开关，在第 i 步之后点亮的位置集合的异或和
    for (int i = 1; i <= 3 * n; ++i) {
        pad ^= 1 << ((i - 1) % n);
        for (int mask = 0; mask < full; ++mask) {
            if (can[i - 1][rep[mask]]) {
                // 由旋转对称性可以固定第一步拨动 0 号开关：它在之后 i 步里
                // 依次点亮 0..i-1 号灯，即 pad；剩下的 i-1 步构成子问题。
                can[i][rep[pad ^ mask]] = true;
            }
        }
    }

    char lights[32], switches[32];
    while (t--) {
        scanf("%s%s", lights, switches);
        int fin = strToMask(lights);
        int pad = strToMask(switches);   // 未拨动的那些开关自己产生的影响
        int cur = 0;
        // 构造性上界是 3n 步，答案一定在这个范围内
        for (int i = 0; i <= 3 * n; ++i) {
            if (can[i][rep[cur ^ fin]]) {
                printf("%d\n", i);
                break;
            }
            cur ^= pad;              // 这一步所有激活开关对灯的影响
            pad = cycRight(pad);     // 开关整体右旋一位，进入下一步
        }
    }
    return 0;
}
