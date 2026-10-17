#include <iostream>
#include <string>
#include <vector>

struct Sample {
    std::string bits;
    int result;
};

void Solve() {
    int n, m;
    std::cin >> n >> m;
    std::vector<Sample> samples(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> samples[i].bits >> samples[i].result;
    }

    std::vector<bool> resolved(m, false);
    int remaining = m;

    // 反向解析: 每次寻找一条合法的 if 规则 "if (b[var] == val) return res;"
    // 使得在该规则下匹配的所有样本的输出结果完全一致!
    // 匹配的样本即可从剩余待判断集合中剔除.
    while (remaining > 0) {
        bool foundRule = false;

        for (int var = 0; var < n; ++var) {
            for (char val : {'0', '1'}) {
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
                            resolved[i] = true;
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
            std::cout << "LIE\n";
            return;
        }
    }

    std::cout << "OK\n";
}

int main() {
    int t;
    if (std::cin >> t) {
        while (t--) {
            Solve();
        }
    }
    return 0;
}
