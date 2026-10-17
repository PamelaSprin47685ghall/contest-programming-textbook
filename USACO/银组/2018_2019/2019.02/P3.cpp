#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

const int MAXN = 100000;

int pastureCount, cowCount;
vector<int> sameNbrs[MAXN + 1];     // 要求两端种同一种草的邻居
vector<int> diffNbrs[MAXN + 1];     // 要求两端种不同草的邻居
int grass[MAXN + 1];                // 0 表示还没种, 1 和 2 表示两种草
bool impossible = false;

int main() {
    scanf("%d%d", &pastureCount, &cowCount);
    for (int i = 0; i < cowCount; i++) {
        char kind;
        int a, b;
        scanf(" %c %d %d", &kind, &a, &b);
        if (kind == 'S') {
            sameNbrs[a].push_back(b);
            sameNbrs[b].push_back(a);
        } else {
            diffNbrs[a].push_back(b);
            diffNbrs[b].push_back(a);
        }
    }

    int componentCount = 0;
    for (int start = 1; start <= pastureCount; start++) {
        if (grass[start] != 0) continue;

        // 每进一个新起点就是一个新的连通块. 块内先随便给起点定一种草,
        // 其余牧场的草种随后被两种边逼成唯一, 因此整块只有"原样"和"整体翻转"两种方案.
        componentCount++;
        grass[start] = 1;
        queue<int> pending;
        pending.push(start);
        while (!pending.empty()) {
            int x = pending.front();
            pending.pop();
            for (int i = 0; i < (int)sameNbrs[x].size(); i++) {
                int y = sameNbrs[x][i];
                // 同种草却已经被逼成另一种, 说明要求自相矛盾
                if (grass[y] == 3 - grass[x]) impossible = true;
                if (grass[y] == 0) {
                    grass[y] = grass[x];
                    pending.push(y);
                }
            }
            for (int i = 0; i < (int)diffNbrs[x].size(); i++) {
                int y = diffNbrs[x][i];
                // 要求不同草却已经和 x 同色, 同样矛盾
                if (grass[y] == grass[x]) impossible = true;
                if (grass[y] == 0) {
                    grass[y] = 3 - grass[x];
                    pending.push(y);
                }
            }
        }
    }

    if (impossible) {
        printf("0\n");
    } else {
        // 每个连通块可以整体翻转, 方案数为 2 的块数次方, 写成二进制就是 1 后面跟 K 个 0
        printf("1");
        for (int i = 0; i < componentCount; i++) {
            printf("0");
        }
        printf("\n");
    }
    return 0;
}
