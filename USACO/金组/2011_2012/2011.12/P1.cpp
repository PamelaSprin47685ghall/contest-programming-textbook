#include <algorithm>
#include <cstdio>
#include <map>

using namespace std;

const int MAXN = 20005;

int pos[5][MAXN];  // pos[p][c]: 压缩编号为 c 的牛在第 p 张照片里的位置
int realId[MAXN];  // 压缩编号 c 对应的真实编号

// 判断两头牛谁应排在前面: 若 a 在至少 3 张照片里排在 b 前面, 就认为原序中 a 在 b 前.
// 理由: 每头牛最多只在一张照片里主动移动, 所以任意两头牛的真实相对顺序
// 最多被两张照片破坏, 剩下至少三张照片仍然如实反映原序.
bool cmp(int a, int b) {
    int wins = 0;
    for (int p = 0; p < 5; p++) {
        if (pos[p][a] < pos[p][b]) {
            wins++;
        }
    }
    return wins >= 3;
}

int main() {
    int n;
    scanf("%d", &n);

    // 牛的编号可达 1e9, 按首次出现的顺序压缩成 0..n-1 才能塞进数组
    map<int, int> idOf;
    int cnt = 0;
    for (int p = 0; p < 5; p++) {
        for (int i = 0; i < n; i++) {
            int cow;
            scanf("%d", &cow);
            if (idOf.find(cow) == idOf.end()) {
                realId[cnt] = cow;
                idOf[cow] = cnt;
                cnt++;
            }
            pos[p][idOf[cow]] = i;
        }
    }

    // 用上面的比较器直接排序, 得到的就是原始顺序
    int order[MAXN];
    for (int i = 0; i < n; i++) {
        order[i] = i;
    }
    sort(order, order + n, cmp);

    for (int i = 0; i < n; i++) {
        printf("%d\n", realId[order[i]]);
    }
    return 0;
}
