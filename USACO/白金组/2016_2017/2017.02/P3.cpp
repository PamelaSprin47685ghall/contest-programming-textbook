#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 100000; // 奶牛品种数上限

int n, k;
int sideA[MAXN + 1]; // 一侧字段的品种顺序
int sideB[MAXN + 1]; // 另一侧字段的品种顺序
int posA[MAXN + 1];  // 每个品种在 A 侧的位置
int posB[MAXN + 1];  // 每个品种在 B 侧的位置
int otherPos[MAXN + 1]; // otherPos[i]：A 侧第 i 个字段的品种在 B 侧的位置

// 二维计数结构：外层按下标（A 侧位置）分段的树状数组，每个外层节点挂着
// 该段内全部 y 值的有序表，再配一个内层树状数组记录其中被激活的个数。
vector<int> nodeValues[MAXN + 1];
vector<int> nodeBit[MAXN + 1];
int nodeActive[MAXN + 1];

int invBit[MAXN + 1]; // 统计总逆序对用的树状数组

void invAdd(int x) {
    for (; x <= n; x += x & -x) ++invBit[x];
}

int invSum(int x) {
    int s = 0;
    for (; x > 0; x -= x & -x) s += invBit[x];
    return s;
}

void innerAdd(int node, int idx) {
    for (int t = idx; t < (int)nodeBit[node].size(); t += t & -t) ++nodeBit[node][t];
}

int innerSum(int node, int idx) {
    int s = 0;
    for (int t = idx; t > 0; t -= t & -t) s += nodeBit[node][t];
    return s;
}

// 激活点 (x, y)（每个点只激活一次）
void activate(int x, int y) {
    for (int j = x; j <= n; j += j & -j) {
        int idx = (int)(lower_bound(nodeValues[j].begin(), nodeValues[j].end(), y) - nodeValues[j].begin()) + 1;
        innerAdd(j, idx);
        ++nodeActive[j];
    }
}

// 统计已激活的点中 x < X 且 y > Y 的个数
long long countTopLeft(int X, int Y) {
    long long res = 0;
    for (int j = X - 1; j > 0; j -= j & -j) {
        // 节点 j 的有序表里 <= Y 的值有 pos 个，用内层树状数组数出其中已激活几个
        int pos = (int)(upper_bound(nodeValues[j].begin(), nodeValues[j].end(), Y) - nodeValues[j].begin());
        res += nodeActive[j] - innerSum(j, pos);
    }
    return res;
}

struct Event {
    int sign; // +1：统计品种编号 <= 上限的点；-1：扣掉更早那一小段
    int x, y;
};
vector<Event> events[MAXN + 1];

int main() {
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; ++i) scanf("%d", &sideA[i]);
    for (int i = 1; i <= n; ++i) scanf("%d", &sideB[i]);
    for (int i = 1; i <= n; ++i) posA[sideA[i]] = i;
    for (int i = 1; i <= n; ++i) posB[sideB[i]] = i;

    // 总交叉对数：把 A 侧第 i 个字段的品种换成它在 B 侧的位置，交叉对数就是该序列的逆序对数
    long long total = 0;
    for (int i = 1; i <= n; ++i) {
        otherPos[i] = posB[sideA[i]];
        total += (i - 1) - invSum(otherPos[i]);
        invAdd(otherPos[i]);
    }

    // 把"品种 v 就是一个点 (posA[v], posB[v])"登记进外层树状数组的各个节点
    for (int i = 1; i <= n; ++i) {
        for (int j = i; j <= n; j += j & -j) nodeValues[j].push_back(otherPos[i]);
    }
    for (int j = 1; j <= n; ++j) {
        sort(nodeValues[j].begin(), nodeValues[j].end());
        nodeBit[j].assign(nodeValues[j].size() + 1, 0);
    }

    // 友好交叉对 = 对每个品种 c，统计点 a 落在 c 点左上角、且品种编号 a <= c+k 的个数，
    // 再减去其中 a <= c-k-1 的个数（这两类之差正好是 |a-c| <= k 且交叉的有序对数）。
    // 按品种编号从小到大扫描，扫到 v 时激活品种 v 的点，然后回答挂在 v 上的询问。
    for (int c = 1; c <= n; ++c) {
        int upper = c + k <= n ? c + k : n;
        Event up;
        up.sign = 1;
        up.x = posA[c];
        up.y = posB[c];
        events[upper].push_back(up);
        if (c - k - 1 >= 1) {
            Event down;
            down.sign = -1;
            down.x = posA[c];
            down.y = posB[c];
            events[c - k - 1].push_back(down);
        }
    }

    long long friendly = 0;
    for (int v = 1; v <= n; ++v) {
        activate(posA[v], posB[v]);
        for (int e = 0; e < (int)events[v].size(); ++e) {
            friendly += (long long)events[v][e].sign * countTopLeft(events[v][e].x, events[v][e].y);
        }
    }

    // 不合群的交叉对 = 全部交叉对 - 友好交叉对
    printf("%lld\n", total - friendly);
    return 0;
}
