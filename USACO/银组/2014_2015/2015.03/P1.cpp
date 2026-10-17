#include <cstdio>

using namespace std;

// cnt[c][r]: 变量 c 的候选值里, 模 7 余 r 的有多少个
// c 直接用字符的 ASCII 码当下标, 七种变量各占一行
long long cnt[128][7];

int main() {
    int N;
    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        char name;
        int value;
        scanf(" %c %d", &name, &value);
        // 取值可以是负数, C++ 里负数直接取模会得到负余数,
        // 先加 7 再取模, 把下标压回 [0, 6]
        // 变量名字符的编码一定非负, 转成 unsigned char 后可以放心当下标
        cnt[(unsigned char)name][(value % 7 + 7) % 7]++;
    }

    long long result = 0;
    // 乘积是不是 7 的倍数, 只取决于七个变量各自模 7 的余数,
    // 所以 500^7 种取值只需按 7^7 种余数组合枚举一遍
    for (int B = 0; B < 7; B++) {
        for (int E = 0; E < 7; E++) {
            for (int S = 0; S < 7; S++) {
                for (int I = 0; I < 7; I++) {
                    for (int G = 0; G < 7; G++) {
                        for (int O = 0; O < 7; O++) {
                            for (int M = 0; M < 7; M++) {
                                if (((B + E + S + S + I + E) * (G + O + E + S) * (M + O + O)) % 7 == 0) {
                                    // 每个变量在各自余数类里任选一个具体取值, 都是不同的方案, 用乘法原理累加
                                    result += cnt['B'][B] * cnt['E'][E] * cnt['S'][S] * cnt['I'][I] *
                                              cnt['G'][G] * cnt['O'][O] * cnt['M'][M];
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    printf("%lld\n", result);
    return 0;
}
