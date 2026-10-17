#include <cstdio>

using namespace std;

const int MAXN = 500005;

int n, m;
int maxGap[4 * MAXN];    // 节点管辖范围内最长的一段连续空座
int leftGap[4 * MAXN];   // 紧贴节点左端点的连续空座数
int rightGap[4 * MAXN];  // 紧贴节点右端点的连续空座数
unsigned char state[4 * MAXN];  // 0: 整段全空, 1: 整段全满, 2: 混合(必须看子节点)

void build(int node, int l, int r) {
    maxGap[node] = leftGap[node] = rightGap[node] = r - l + 1;
    state[node] = 0;
    if (l == r) return;
    int mid = (l + r) / 2;
    build(node * 2, l, mid);
    build(node * 2 + 1, mid + 1, r);
}

// 整段统一置空(0)或坐满(1): 有了这个标记就不必往下走到每个叶子
void setWhole(int node, int l, int r, int kind) {
    int len = r - l + 1;
    if (kind == 0) maxGap[node] = leftGap[node] = rightGap[node] = len;
    else maxGap[node] = leftGap[node] = rightGap[node] = 0;
    state[node] = (unsigned char)kind;
}

void pushDown(int node, int l, int r) {
    if (state[node] == 2 || l == r) return;
    int mid = (l + r) / 2;
    setWhole(node * 2, l, mid, state[node]);
    setWhole(node * 2 + 1, mid + 1, r, state[node]);
    state[node] = 2;  // 标记只对整段有效, 下传后子节点的值才是有效的
}

void pullUp(int node, int l, int r) {
    int mid = (l + r) / 2;
    int leftLen = mid - l + 1, rightLen = r - mid;
    int lc = node * 2, rc = node * 2 + 1;

    maxGap[node] = maxGap[lc] > maxGap[rc] ? maxGap[lc] : maxGap[rc];
    // 跨过中点的空段 = 左儿子的后缀空段 + 右儿子的前缀空段 (注意是右左, 别写反)
    if (rightGap[lc] + leftGap[rc] > maxGap[node])
        maxGap[node] = rightGap[lc] + leftGap[rc];

    leftGap[node] = leftGap[lc];
    if (leftGap[lc] == leftLen) leftGap[node] = leftLen + leftGap[rc];  // 左半全空才能接过右半

    rightGap[node] = rightGap[rc];
    if (rightGap[rc] == rightLen) rightGap[node] = rightLen + rightGap[lc];

    state[node] = 2;
}

// 把 [ql, qr] 全部置空(kind = 0)或全部坐满(kind = 1)
void update(int node, int l, int r, int ql, int qr, int kind) {
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) {  // 整段被覆盖, 直接打标记
        setWhole(node, l, r, kind);
        return;
    }
    pushDown(node, l, r);
    int mid = (l + r) / 2;
    update(node * 2, l, mid, ql, qr, kind);
    update(node * 2 + 1, mid + 1, r, ql, qr, kind);
    pullUp(node, l, r);
}

// 找出最靠左的、能容纳 need 个连续空座的起点; 找不到时调用方已经用 maxGap[1] 拦住了
int findSeat(int node, int l, int r, int need) {
    if (state[node] == 0) return l;  // 整段全空, 最左端就是答案
    if (l == r) return l;
    pushDown(node, l, r);
    int mid = (l + r) / 2;
    int lc = node * 2, rc = node * 2 + 1;
    // 左半边的任何位置都比右半边靠左, 所以优先往左走
    if (maxGap[lc] >= need) return findSeat(lc, l, mid, need);
    // 左半边装不下, 但跨过中点的空段可能够; 它的起点就是中点往左数 rightGap[lc] 个位置
    if (rightGap[lc] + leftGap[rc] >= need) return mid - rightGap[lc] + 1;
    return findSeat(rc, mid + 1, r, need);
}

int main() {
    scanf("%d %d", &n, &m);
    build(1, 1, n);

    int turnedAway = 0;
    char op[4];
    for (int i = 0; i < m; i++) {
        scanf("%s", op);
        if (op[0] == 'A') {
            int p;
            scanf("%d", &p);
            if (maxGap[1] < p) {  // 全场最长的空段都坐不下, 只能拒之门外
                turnedAway++;
                continue;
            }
            int pos = findSeat(1, 1, n, p);
            update(1, 1, n, pos, pos + p - 1, 1);
        } else {
            int a, b;
            scanf("%d %d", &a, &b);
            update(1, 1, n, a, b, 0);
        }
    }

    printf("%d\n", turnedAway);
    return 0;
}
