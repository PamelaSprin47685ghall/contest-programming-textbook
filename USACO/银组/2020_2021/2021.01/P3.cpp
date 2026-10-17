#include <cstdio>

using namespace std;

const int MAXN = 1000;

int n;
int beauty[MAXN + 5][MAXN + 5];

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            scanf("%d", &beauty[i][j]);

    // 合法摆放只有两类：要么每一行都按"放、空、放、空……"交替，
    // 要么每一列都按交替。分类之后各行（各列）的相位可以独立选择。

    // 每一行内部交替：逐行挑两种相位里更优的那个
    long long rowsCase = 0;
    for (int i = 1; i <= n; ++i) {
        long long oddCols = 0, evenCols = 0;  // 分别对应"从奇数列开始放"和"从偶数列开始放"
        for (int j = 1; j <= n; ++j) {
            if (j & 1) oddCols += beauty[i][j];
            else evenCols += beauty[i][j];
        }
        rowsCase += (oddCols > evenCols ? oddCols : evenCols);
    }

    // 每一列内部交替：逐列挑两种相位里更优的那个
    long long colsCase = 0;
    for (int j = 1; j <= n; ++j) {
        long long oddRows = 0, evenRows = 0;
        for (int i = 1; i <= n; ++i) {
            if (i & 1) oddRows += beauty[i][j];
            else evenRows += beauty[i][j];
        }
        colsCase += (oddRows > evenRows ? oddRows : evenRows);
    }

    printf("%lld\n", rowsCase > colsCase ? rowsCase : colsCase);
    return 0;
}
