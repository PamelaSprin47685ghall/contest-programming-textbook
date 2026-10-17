#include <cstdio>
#include <vector>
using namespace std;

// 坐标不超过 2N <= 2*10^5，用 10^9 当"无穷大"足够
const int INF = 1000000000;

// 线段树：把每个仍然在场的箱子的左下角记为 a[x1] = y1，
// 支持"单点修改"和"前缀最小值（附带回传取得最小值的箱子编号）"。
int S;                 // 叶子个数，取成不小于坐标范围的 2 的幂
vector<int> mnY, mnId; // 每个结点管辖区间内最小的 y1 及其对应的箱子编号

// 把位置 p 上的信息改成 (y, id)；y = INF 表示该位置上的箱子已被搬走
void setv(int p, int y, int id) {
    p += S;
    mnY[p] = y;
    mnId[p] = id;
    for (p >>= 1; p >= 1; p >>= 1) {
        int l = p << 1, r = l | 1;
        // 与官方写法一致：相等时取右边的结点（各箱子 y1 互不相同，其实无影响）
        if (mnY[l] < mnY[r]) {
            mnY[p] = mnY[l];
            mnId[p] = mnId[l];
        } else {
            mnY[p] = mnY[r];
            mnId[p] = mnId[r];
        }
    }
}

// 查询 [0, r) 内 y1 最小的箱子编号；区间内没有箱子时返回 -1
int queryMin(int r) {
    int best = INF, bid = -1;
    int l = S;
    r += S;
    while (l < r) {
        if (l & 1) {
            if (mnY[l] < best) {
                best = mnY[l];
                bid = mnId[l];
            }
            ++l;
        }
        if (r & 1) {
            --r;
            if (mnY[r] < best) {
                best = mnY[r];
                bid = mnId[r];
            }
        }
        l >>= 1;
        r >>= 1;
    }
    return bid;
}

int main() {
    int T, M;
    scanf("%d %d", &T, &M);
    vector<int> X1, Y1, X2, Y2;
    vector<int> stk;      // 手写栈：模拟 DFS 的递归栈，避免 N 很大时爆栈
    vector<int> ans;      // M = 1 的答案序列
    vector<char> done;    // M = 1 中该箱子是否已经进入答案序列
    vector<char> out;     // M = 2 的输出缓冲
    while (T--) {
        int N;
        scanf("%d", &N);
        X1.assign(N, 0);
        Y1.assign(N, 0);
        X2.assign(N, 0);
        Y2.assign(N, 0);
        for (int i = 0; i < N; ++i) {
            int a, b, c, d;
            scanf("%d %d %d %d", &a, &b, &c, &d);
            X1[i] = a - 1; // 全部化成 0 下标坐标
            Y1[i] = b - 1;
            X2[i] = c - 1;
            Y2[i] = d - 1;
        }
        S = 1;
        while (S < 2 * N) S <<= 1; // 位置 0..2N-1
        mnY.assign(2 * S, INF);
        mnId.assign(2 * S, -1);
        for (int i = 0; i < N; ++i) setv(X1[i], Y1[i], i);

        if (M == 1) {
            // 阻挡关系 i->j 构成一张有向无环图，需要给出一个拓扑序。
            // 直接按拓扑序输出会枚举太多边，于是用 DFS 版拓扑排序：
            // 每次只在"仍然在场"的箱子里找那个 y1 最小的阻挡者继续递归，
            // 递归结束后它一定已经入答案并被搬走，因此同一箱子只会被递归到一次。
            ans.clear();
            done.assign(N, 0);
            stk.clear();
            for (int s = 0; s < N; ++s) {
                if (done[s]) continue;
                stk.push_back(s);
                while (!stk.empty()) {
                    int x = stk.back();
                    // 先把自己移出线段树，否则可能把自己当成阻挡自己的箱子
                    setv(X1[x], INF, -1);
                    int y = queryMin(X2[x]);
                    if (y < 0 || Y1[y] >= Y2[x]) {
                        // 西南方向已无别的箱子，现在就能把它搬走
                        ans.push_back(x);
                        done[x] = 1;
                        stk.pop_back(); // 保持"已搬走"状态
                    } else {
                        // 还有箱子 y 挡着：先恢复自己，等 y 被搬走后再回来处理
                        setv(X1[x], Y1[x], x);
                        stk.push_back(y);
                    }
                }
            }
            for (int i = 0; i < N; ++i) {
                if (i) printf(" ");
                printf("%d", ans[i] + 1);
            }
            printf("\n");
        } else {
            // M = 2：按编号顺序处理，第 k 个箱子查询时 1..k-1 已被搬走，
            // 所以直接把它自己删掉再查前缀最小值即可。
            out.assign(N, '0');
            for (int i = 0; i < N; ++i) {
                setv(X1[i], INF, -1);
                int y = queryMin(X2[i]);
                if (y < 0 || Y1[y] >= Y2[i]) out[i] = '1';
            }
            out.push_back('\0');
            printf("%s\n", &out[0]);
        }
    }
    return 0;
}
