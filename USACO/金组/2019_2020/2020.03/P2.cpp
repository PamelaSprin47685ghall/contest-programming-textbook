#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 200005;

int N, M;
int parent[MAXN];            // 并查集：被判定必须同色的牛最终合并成一组
int groupSize[MAXN];         // 每组的牛数，「小并大」时用来决定搬动哪一边
vector<int> admirers[MAXN];  // admirers[r]：组 r 的成员各自被哪些牛仰慕
vector<int> toCheck;         // 待重新检查的组；只用一个下标 head 前进，不真正出队
int head = 0;
int colorOf[MAXN];           // 每个组分配到的颜色编号

int findRoot(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];  // 路径压缩
        x = parent[x];
    }
    return x;
}

void mergeGroups(int a, int b) {
    a = findRoot(a);
    b = findRoot(b);
    if (a == b) return;
    // 小并大：保证名单里的每个元素最多被搬 O(log N) 次
    if (groupSize[a] < groupSize[b]) {
        int temp = a;
        a = b;
        b = temp;
    }
    parent[b] = a;
    groupSize[a] += groupSize[b];
    // 两组合并后，仰慕其中任意一方的牛也必须同色，所以名单要并到一起
    for (int i = 0; i < (int)admirers[b].size(); i++) {
        admirers[a].push_back(admirers[b][i]);
    }
    admirers[b].clear();
    if (admirers[a].size() > 1) toCheck.push_back(a);
}

int main() {
    scanf("%d %d", &N, &M);
    for (int i = 1; i <= N; i++) {
        parent[i] = i;
        groupSize[i] = 1;
    }
    for (int e = 0; e < M; e++) {
        int a, b;
        scanf("%d %d", &a, &b);
        admirers[a].push_back(b);
    }
    for (int i = 1; i <= N; i++) {
        if (admirers[i].size() > 1) toCheck.push_back(i);
    }

    while (head < (int)toCheck.size()) {
        int x = toCheck[head];
        if (admirers[x].size() <= 1) {
            head++;
            continue;
        }
        // 同一个组被两头来自不同组的牛仰慕时，这两头牛必然同色
        int first = admirers[x].back();
        admirers[x].pop_back();
        // 剩下的记录若与 first 同组，它仍然代表这个组，重复记录可以丢掉；
        // 必须至少保留一条记录，否则这个组以后再次合并时就查不到了
        if (findRoot(first) == findRoot(admirers[x].back())) continue;
        mergeGroups(first, admirers[x].back());
    }

    // 过程停下时每组的名单里至多剩一个不同的组，此时给每个组分配一种
    // 与其他组都不同的颜色必然满足条件，而且组数就是最大颜色数。
    // 按编号从小到大首次遇到新组时分配下一个新颜色，得到字典序最小的一组。
    int usedColors = 0;
    for (int i = 1; i <= N; i++) {
        int root = findRoot(i);
        if (colorOf[root] == 0) colorOf[root] = ++usedColors;
        printf("%d\n", colorOf[root]);
    }
    return 0;
}
