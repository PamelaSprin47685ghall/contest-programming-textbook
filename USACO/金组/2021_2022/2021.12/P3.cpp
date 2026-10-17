#include <cstdio>
#include <vector>
using namespace std;

// N <= 50, M <= 50，四组判定条件逐一检查即可。
int N, M;
int lastLine[55];            // 颜色上一次出现在哪条光束上，0 表示还从未出现
int parent[55];              // 包含树中的父颜色，0 表示挂在最外层（占位节点）
vector<int> colorsOf[55];    // 每条光束上扫到的颜色序列
int firstPos[55][55];        // firstPos[i][c]：颜色 c 在光束 i 上第一次出现的位置（从 1 起），0 表示没出现

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d %d", &N, &M);
        bool good = true;
        for (int c = 1; c <= N; c++) {
            lastLine[c] = 0;
            parent[c] = 0;
        }
        for (int i = 1; i <= M; i++) {
            colorsOf[i].clear();
            int k;
            scanf("%d", &k);
            vector<int> stk;     // 当前“还开着”的颜色栈，用于判定区间只许嵌套或相离
            int cur = 0;         // 当前把新颜色夹在里面的最内层颜色
            for (int j = 0; j < k; j++) {
                int c;
                scanf("%d", &c);
                colorsOf[i].push_back(c);
                if (!good) continue;   // 已经判 NO，只把输入读完
                if (lastLine[c] == i) {
                    // 本光束上的第二次出现：它必须正好还在栈顶，否则两个区间会“交叉”
                    if (stk.empty() || stk.back() != c) {
                        good = false;
                        continue;
                    }
                    stk.pop_back();
                    cur = parent[c];
                } else {
                    // 本光束上的第一次出现
                    // 条件 1：出现过的光束编号必须连续
                    if (lastLine[c] != 0 && lastLine[c] != i - 1) good = false;
                    // 条件 3：同一颜色在所有光束上被夹住它的“父颜色”必须一致
                    if (lastLine[c] != 0 && parent[c] != cur) good = false;
                    parent[c] = cur;
                    lastLine[c] = i;
                    stk.push_back(c);
                    cur = c;
                }
            }
        }
        // 条件 4：任意两种颜色在任意两条共同出现的光束上，上下相对次序必须相同。
        // 用“第一次出现的位置”作为次序关键字：合法时它等价于两条手镯的上下关系。
        for (int i = 1; i <= M; i++)
            for (int c = 1; c <= N; c++) firstPos[i][c] = 0;
        for (int i = 1; i <= M; i++)
            for (int j = 0; j < (int)colorsOf[i].size(); j++) {
                int c = colorsOf[i][j];
                if (firstPos[i][c] == 0) firstPos[i][c] = j + 1;
            }
        for (int a = 1; a <= N && good; a++)
            for (int b = a + 1; b <= N && good; b++) {
                int sign = 0;   // 0 未定，-1 表示 a 更靠下，1 表示 b 更靠下
                for (int i = 1; i <= M; i++) {
                    if (firstPos[i][a] && firstPos[i][b]) {
                        int s = (firstPos[i][a] < firstPos[i][b]) ? -1 : 1;
                        if (sign == 0) sign = s;
                        else if (sign != s) good = false;
                    }
                }
            }
        printf(good ? "YES\n" : "NO\n");
    }
    return 0;
}
