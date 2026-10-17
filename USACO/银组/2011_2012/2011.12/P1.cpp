#include <algorithm>
#include <cstdio>
#include <map>

using namespace std;

const int MAXN = 20005;

int n;
int pos[5][MAXN];  // pos[p][c] 表示紧凑编号为 c 的牛在第 p 张照片里站的位置
int realId[MAXN];  // 紧凑编号 c 对应的真实编号

// 比较两头牛: 若其中一头在至少 3 张照片里排在另一头前面, 它就应排在前面.
// 依据是每头牛最多只在一张照片里主动移动, 所以任意两头牛的真实相对顺序
// 最多被 2 张照片破坏, 剩下至少 3 张照片仍保持原样.
bool cmp(int a, int b) {
    int wins = 0;
    for (int p = 0; p < 5; p++)
        if (pos[p][a] < pos[p][b]) wins++;
    return wins >= 3;
}

int main() {
    scanf("%d", &n);

    // 牛的真实编号可达 1e9, 先按首次出现的顺序压缩成 0..n-1
    map<int, int> id;
    int cnt = 0;
    for (int p = 0; p < 5; p++) {
        for (int i = 0; i < n; i++) {
            int cow;
            scanf("%d", &cow);
            if (id.find(cow) == id.end()) {
                realId[cnt] = cow;
                id[cow] = cnt++;
            }
            pos[p][id[cow]] = i;
        }
    }

    int order[MAXN];
    for (int i = 0; i < n; i++) order[i] = i;
    sort(order, order + n, cmp);

    for (int i = 0; i < n; i++) printf("%d\n", realId[order[i]]);
    return 0;
}
