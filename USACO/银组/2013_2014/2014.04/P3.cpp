#include <cstdio>

using namespace std;

// 10^18 共有 19 位数
const int MAXDIGIT = 20;

long long combine[MAXDIGIT][MAXDIGIT];  // combine[n][k] 是 C(n, k)
long long powerOfNine[MAXDIGIT];        // 9 的幂: 一个自由数位放"非目标数字"的选择数

int limitDigit[MAXDIGIT];  // 上限的各位数字, 下标 0 是最高位

// 把 value 的十进制各位写进 digit(高位在前), 返回位数
int SplitDigits(long long value, int digit[]) {
    int reversed[MAXDIGIT];
    int length = 0;
    while (value > 0) {
        reversed[length] = (int)(value % 10);
        length++;
        value /= 10;
    }
    for (int i = 0; i < length; i++) {
        digit[i] = reversed[length - 1 - i];
    }
    return length;
}

// freeCount 个自由数位里至少有 need 位是目标数字的方案数;
// 其余数位可以放除目标数字外的 9 种数字
long long FreeFillings(int freeCount, int need) {
    if (need < 0) {
        need = 0;
    }
    long long total = 0;
    for (int k = need; k <= freeCount; k++) {
        total += combine[freeCount][k] * powerOfNine[freeCount - k];
    }
    return total;
}

// 统计 [1, limit] 中, 数字 target 出现次数占一半以上的数:
// 条件是 2 * 出现次数 >= 位数, 即出现次数 >= (位数 + 1) / 2
long long CountWithDigitMajority(long long limit, int target) {
    if (limit <= 0) {
        return 0;
    }
    int length = SplitDigits(limit, limitDigit);
    long long total = 0;
    for (int len = 1; len <= length; len++) {
        int needTotal = (len + 1) / 2;
        if (len < length) {
            // 位数更少的数一定小于 limit, 只需保证首位不是 0
            for (int first = 1; first <= 9; first++) {
                int need = needTotal - (first == target ? 1 : 0);
                total += FreeFillings(len - 1, need);
            }
        } else {
            // 位数相同: 枚举"第一个比 limit 小的位置"pos,
            // 只要求与 limit 不同的那一类数, 所以不会重复
            int used = 0;  // 前缀里已经出现的 target 个数
            for (int pos = 0; pos < length; pos++) {
                int low = (pos == 0 ? 1 : 0);
                for (int digit = low; digit < limitDigit[pos]; digit++) {
                    int need = needTotal - used - (digit == target ? 1 : 0);
                    total += FreeFillings(length - 1 - pos, need);
                }
                used += (limitDigit[pos] == target ? 1 : 0);
            }
            if (2 * used >= length) {
                total++;  // limit 自己也算
            }
        }
    }
    return total;
}

// 恰好由两种数字组成、每种都出现一半的数, 位数必为偶数.
// 它们在上面会分别被两种数字各数一次, 所以要再减去一份
long long CountTwoDigitsHalfHalf(long long limit) {
    if (limit <= 0) {
        return 0;
    }
    int length = SplitDigits(limit, limitDigit);
    long long total = 0;
    for (int len = 2; len <= length; len += 2) {
        int half = len / 2;
        for (int small = 0; small <= 9; small++) {
            for (int large = small + 1; large <= 9; large++) {
                if (len < length) {
                    // 位数更少的数一定小于 limit, 只需保证首位不是 0
                    if (small == 0) {
                        // 首位只能放 large, 剩下 len-1 位里正好一半放 0
                        total += combine[len - 1][half];
                    } else {
                        total += combine[len][half];
                    }
                    continue;
                }
                // 位数相同: 同样枚举"第一个比 limit 小的位置"
                int usedSmall = 0;
                int usedLarge = 0;
                for (int pos = 0; pos < length; pos++) {
                    int free = length - 1 - pos;
                    for (int which = 0; which < 2; which++) {
                        int digit = (which == 0 ? small : large);
                        if (digit >= limitDigit[pos]) {
                            continue;
                        }
                        if (pos == 0 && digit == 0) {
                            continue;  // 首位不能是 0
                        }
                        int nextSmall = usedSmall + (digit == small ? 1 : 0);
                        int nextLarge = usedLarge + (digit == large ? 1 : 0);
                        // 两种数字各要一半; 自由位只能用这两种数字, 个数是定死的
                        if (nextSmall <= half && nextLarge <= half) {
                            total += combine[free][half - nextSmall];
                        }
                    }
                    if (limitDigit[pos] != small && limitDigit[pos] != large) {
                        break;  // 这一位取不了等, 后面的位置更不可能取等
                    }
                    usedSmall += (limitDigit[pos] == small ? 1 : 0);
                    usedLarge += (limitDigit[pos] == large ? 1 : 0);
                }
                if (usedSmall == half && usedLarge == half) {
                    total++;  // limit 自己也算
                }
            }
        }
    }
    return total;
}

// 统计 [1, limit] 中"至少一半数位相同"的数的个数
long long CountInteresting(long long limit) {
    long long total = 0;
    for (int target = 0; target <= 9; target++) {
        total += CountWithDigitMajority(limit, target);
    }
    return total - CountTwoDigitsHalfHalf(limit);
}

int main() {
    for (int n = 0; n < MAXDIGIT; n++) {
        combine[n][0] = 1;
        combine[n][n] = 1;
        for (int k = 1; k < n; k++) {
            combine[n][k] = combine[n - 1][k - 1] + combine[n - 1][k];
        }
    }
    powerOfNine[0] = 1;
    for (int i = 1; i < MAXDIGIT; i++) {
        powerOfNine[i] = powerOfNine[i - 1] * 9;
    }

    long long X, Y;
    scanf("%lld %lld", &X, &Y);
    printf("%lld\n", CountInteresting(Y) - CountInteresting(X - 1));
    return 0;
}
