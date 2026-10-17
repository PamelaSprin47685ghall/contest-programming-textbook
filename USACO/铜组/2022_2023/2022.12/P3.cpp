#include <cstdio>
#include <string>
#include <vector>

using namespace std;

struct Sample {
    string bits;
    int result;
};

void solve() {
    int n, m;
    scanf("%d %d", &n, &m);
    vector<Sample> samples(m);
    char buffer[105];
    for (int i = 0; i < m; ++i) {
        scanf("%s %d", buffer, &samples[i].result);
        samples[i].bits = buffer;
    }

    vector<char> resolved(m, 0);
    int remaining = m;

    // 反向解析: 每次寻找一条合法的 if 规则 "if (b[var] == val) return res;"
    // 使得在该规则下匹配的所有样本的输出结果完全一致!
    // 匹配的样本即可从剩余待判断集合中剔除.
    while (remaining > 0) {
        bool foundRule = false;

        for (int var = 0; var < n; ++var) {
            for (int valueIndex = 0; valueIndex < 2; ++valueIndex) {
                char val = valueIndex == 0 ? '0' : '1';
                int matchedResult = -1;
                bool consistent = true;
                int matchCount = 0;

                for (int i = 0; i < m; ++i) {
                    if (!resolved[i] && samples[i].bits[var] == val) {
                        ++matchCount;
                        if (matchedResult == -1) {
                            matchedResult = samples[i].result;
                        } else if (matchedResult != samples[i].result) {
                            consistent = false;
                            break;
                        }
                    }
                }

                if (matchCount > 0 && consistent) {
                    // 该规则能够无矛盾地区分并剔除 matchCount 个样本
                    foundRule = true;
                    for (int i = 0; i < m; ++i) {
                        if (!resolved[i] && samples[i].bits[var] == val) {
                            resolved[i] = 1;
                            --remaining;
                        }
                    }
                    break;
                }
            }
            if (foundRule) break;
        }

        if (!foundRule) {
            // 无法找到任何能一致分类的分支规则
            printf("LIE\n");
            return;
        }
    }

    printf("OK\n");
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) solve();
    return 0;
}
