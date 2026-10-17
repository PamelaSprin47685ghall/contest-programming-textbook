#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

struct Shake {
    int t, x, y;
};

int main() {
    int n, tCount;
    scanf("%d%d", &n, &tCount);
    char buf[128];
    scanf("%s", buf);
    std::string finalState = buf;

    std::vector<Shake> shakes(tCount);
    for (int i = 0; i < tCount; ++i) {
        scanf("%d%d%d", &shakes[i].t, &shakes[i].x, &shakes[i].y);
        --shakes[i].x;  // 转换为 0-based
        --shakes[i].y;
    }

    std::sort(shakes.begin(), shakes.end(), [](const Shake& a, const Shake& b) {
        return a.t < b.t;
    });

    int possiblePatientZeroCount = 0;
    int minK = 1000000000;
    int maxK = -1;

    // 枚举哪头牛可能是 0 号病人 (patient zero)
    for (int patient = 0; patient < n; ++patient) {
        bool canBePatientZero = false;

        // 枚举传染次数 K 的可能值 (0 到 tCount + 1)
        for (int k = 0; k <= tCount + 1; ++k) {
            std::vector<bool> infected(n, false);
            std::vector<int> numShakes(n, 0);

            infected[patient] = true;

            for (const Shake& s : shakes) {
                bool xInfected = infected[s.x];
                bool yInfected = infected[s.y];

                if (xInfected) numShakes[s.x]++;
                if (yInfected) numShakes[s.y]++;

                // 如果 x 已感染且其传染次数尚未超过 K, 则传染给 y
                if (xInfected && numShakes[s.x] <= k) {
                    infected[s.y] = true;
                }
                // 同理若 y 已感染且其传染次数尚未超过 K, 则传染给 x
                if (yInfected && numShakes[s.y] <= k) {
                    infected[s.x] = true;
                }
            }

            // 检查模拟最终状态是否与题目给出的实际状态完全相符
            bool match = true;
            for (int i = 0; i < n; ++i) {
                if (infected[i] != (finalState[i] == '1')) {
                    match = false;
                    break;
                }
            }

            if (match) {
                canBePatientZero = true;
                minK = std::min(minK, k);
                maxK = std::max(maxK, k);
            }
        }

        if (canBePatientZero) {
            ++possiblePatientZeroCount;
        }
    }

    printf("%d %d ", possiblePatientZeroCount, minK);
    if (maxK > tCount) {
        printf("Infinity\n");
    } else {
        printf("%d\n", maxK);
    }

    return 0;
}
