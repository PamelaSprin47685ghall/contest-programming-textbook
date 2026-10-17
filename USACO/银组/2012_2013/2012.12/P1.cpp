#include <algorithm>
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>

using namespace std;

const int MAXN = 1005;

// 每条栅栏的两个端点坐标
int ax[MAXN], ay[MAXN], bx[MAXN], by[MAXN];
// 栅栏所属的封闭多边形编号
int poly[MAXN];
// mark[cow][k] 记录第 cow 头牛相对第 k 个多边形的位置 (0 在外, 1 在内)
char mark[MAXN][MAXN + 1];
char* markPtr[MAXN];
// 共用同一个端点的所有栅栏
map<long long, vector<int> > incident;

long long Key(int x, int y) {
    return (long long)x * 1000001LL + y;  // 坐标不超过 1e6, 该编码不会冲突
}

// 从 (cx,cy) 向右射出的水平射线是否穿过栅栏 (ax,ay)-(bx,by).
// 端点恰好落在射线上时统一算作"在射线上方", 这样一个顶点只会改变一次计数,
// 射线擦过顶点 (两条边都在同侧) 时奇偶性依然正确.
bool CrossRight(long long cx, long long cy, long long ax, long long ay, long long bx, long long by) {
    if ((ay > cy) == (by > cy)) return false;  // 两端点同侧, 该边不跨越射线所在直线
    // 判断交点横坐标是否严格大于 cx: 用乘法代替除法, 按分母 (by-ay) 的正负决定不等号方向
    long long left = (bx - ax) * (cy - ay);
    long long right = (cx - ax) * (by - ay);
    return (by > ay) ? (left > right) : (left < right);
}

bool CmpMark(char* a, char* b) {
    return strcmp(a, b) < 0;
}

int main() {
    int n, c;
    scanf("%d%d", &n, &c);
    for (int i = 0; i < n; ++i) {
        scanf("%d%d%d%d", &ax[i], &ay[i], &bx[i], &by[i]);
        incident[Key(ax[i], ay[i])].push_back(i);
        incident[Key(bx[i], by[i])].push_back(i);
        poly[i] = -1;
    }

    // 每个端点恰好连接两条栅栏, 所以从任意栅栏出发不断"在端点处换另一条栅栏",
    // 一定会绕回起点, 绕出的正是一个封闭多边形.
    int polyCount = 0;
    for (int i = 0; i < n; ++i) {
        if (poly[i] != -1) continue;
        int cur = i;
        long long at = Key(ax[i], ay[i]);  // 当前进入栅栏 cur 的那个端点
        do {
            poly[cur] = polyCount;
            at = (Key(ax[cur], ay[cur]) == at) ? Key(bx[cur], by[cur]) : Key(ax[cur], ay[cur]);
            vector<int>& around = incident[at];
            int next = -1;
            for (size_t k = 0; k < around.size(); ++k) {
                if (around[k] != cur) {
                    next = around[k];
                    break;
                }
            }
            if (next == -1) break;
            cur = next;
        } while (cur != i);
        ++polyCount;
    }

    // 两头牛能互相走到, 当且仅当对每个多边形它们都在同一侧; 于是内外状态向量相同的牛同属一个社区.
    // 一条栅栏穿过射线就翻转它所在多边形的状态, 与射线奇偶相交判定一致.
    for (int cow = 0; cow < c; ++cow) {
        long long cx, cy;
        scanf("%lld%lld", &cx, &cy);
        for (int j = 0; j < n; ++j) {
            mark[cow][poly[j]] ^= CrossRight(cx, cy, ax[j], ay[j], bx[j], by[j]);
        }
        for (int j = 0; j < polyCount; ++j) {
            mark[cow][j] = mark[cow][j] ? '1' : '0';
        }
        mark[cow][polyCount] = '\0';
        markPtr[cow] = mark[cow];
    }

    // 把状态向量当成字符串排序, 相同向量自然会挨在一起, 最长的一段就是最大社区
    sort(markPtr, markPtr + c, CmpMark);
    int best = 0, run = 0;
    for (int i = 0; i < c; ++i) {
        if (i > 0 && strcmp(markPtr[i], markPtr[i - 1]) == 0) {
            ++run;
        } else {
            run = 1;
        }
        if (run > best) best = run;
    }
    printf("%d\n", best);
    return 0;
}
