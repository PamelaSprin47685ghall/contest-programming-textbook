#include <cstdio>

using namespace std;

const int MAXN = 505;
const int MAXM = 55;

int cowCount, positionCount;
int spotty[MAXN][MAXM];     // 有斑点牛每个位置的碱基, 编码为 A=0, C=1, G=2, T=3
int plain[MAXN][MAXM];      // 无斑点牛每个位置的碱基, 编码相同
int appeared[64];           // 有斑点牛身上出现过的三碱基组合 (按四进制编码, 取值范围 0..63)

int encode(char base) {
    if (base == 'A') {
        return 0;
    }
    if (base == 'C') {
        return 1;
    }
    if (base == 'G') {
        return 2;
    }
    return 3;   // 'T'
}

int main() {
    scanf("%d%d", &cowCount, &positionCount);
    char buffer[MAXM];
    for (int i = 0; i < cowCount; i++) {
        scanf("%s", buffer);
        for (int j = 0; j < positionCount; j++) {
            spotty[i][j] = encode(buffer[j]);
        }
    }
    for (int i = 0; i < cowCount; i++) {
        scanf("%s", buffer);
        for (int j = 0; j < positionCount; j++) {
            plain[i][j] = encode(buffer[j]);
        }
    }

    int answer = 0;
    for (int first = 0; first < positionCount; first++) {
        for (int second = first + 1; second < positionCount; second++) {
            for (int third = second + 1; third < positionCount; third++) {
                // 三个位置的碱基串只要同时出现在有斑点牛和无斑点牛身上, 这两个位置的组合就无法区分斑点,
                // 所以先把有斑点牛的碱基串全部登记, 再看无斑点牛是否撞上.
                for (int i = 0; i < cowCount; i++) {
                    int code = spotty[i][first] * 16 + spotty[i][second] * 4 + spotty[i][third];
                    appeared[code] = 1;
                }
                bool usable = true;
                for (int i = 0; i < cowCount && usable; i++) {
                    int code = plain[i][first] * 16 + plain[i][second] * 4 + plain[i][third];
                    if (appeared[code]) {
                        usable = false;
                    }
                }
                for (int i = 0; i < cowCount; i++) {
                    int code = spotty[i][first] * 16 + spotty[i][second] * 4 + spotty[i][third];
                    appeared[code] = 0;
                }
                if (usable) {
                    answer++;
                }
            }
        }
    }
    printf("%d\n", answer);
    return 0;
}
