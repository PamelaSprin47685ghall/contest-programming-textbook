#include <cstdio>
#include <string>
#include <vector>

using namespace std;

char Buffer[80];

int main() {
    int m, n, k;
    scanf("%d%d%d", &m, &n, &k);
    vector<string> grid(m);
    for (string& row : grid) {
        scanf("%s", Buffer);
        row = Buffer;
    }
    // 题目要求把信号放大 k 倍: 字符横向重复 k 次, 行纵向重复 k 次.
    for (const string& row : grid) {
        for (int row_repeat = 0; row_repeat < k; ++row_repeat) {
            for (char cell : row) {
                for (int width = 0; width < k; ++width) {
                    printf("%c", cell);
                }
            }
            printf("\n");
        }
    }
    return 0;
}
