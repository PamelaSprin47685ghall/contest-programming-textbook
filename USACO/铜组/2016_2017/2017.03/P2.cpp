#include <cstdio>

using namespace std;

const int MAXN = 100;
const int MAXM = 100;

char spotty[MAXN][MAXM + 1];
char plain[MAXN][MAXM + 1];

// 把 A/C/G/T 映射到 0..3, 方便用数组记录某一列出现过哪些碱基.
int BaseIndex(char c) {
    if (c == 'A') return 0;
    if (c == 'C') return 1;
    if (c == 'G') return 2;
    return 3;  // 'T'
}

int main() {
    int N, M;
    scanf("%d %d", &N, &M);
    for (int i = 0; i < N; ++i) {
        scanf("%s", spotty[i]);
    }
    for (int i = 0; i < N; ++i) {
        scanf("%s", plain[i]);
    }

    int answer = 0;
    for (int j = 0; j < M; ++j) {
        // 分别记录这一列上, 有斑点的牛和无斑点的牛出现过哪些碱基.
        bool inSpotty[4] = {false, false, false, false};
        bool inPlain[4] = {false, false, false, false};
        for (int i = 0; i < N; ++i) {
            inSpotty[BaseIndex(spotty[i][j])] = true;
            inPlain[BaseIndex(plain[i][j])] = true;
        }

        // 这一列能解释斑点性, 当且仅当没有任何一种碱基
        // 同时出现在两组奶牛中; 否则这两头牛无法区分.
        bool explains = true;
        for (int k = 0; k < 4; ++k) {
            if (inSpotty[k] && inPlain[k]) {
                explains = false;
            }
        }
        if (explains) {
            ++answer;
        }
    }

    printf("%d\n", answer);
    return 0;
}
