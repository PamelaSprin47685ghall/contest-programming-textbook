#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXITEM = 400005;  // 草块与 Nhoj 的牛加起来最多 4·10^5 个
const int MAXGAIN = 400005;  // 每段最多产生"一牛收益"和"第二头牛的额外收益"两个增量

// 把草块和 Nhoj 的牛放进同一个数组按位置排序: tastiness >= 0 表示草块,
// tastiness == -1 表示这里站着一头 Nhoj 的牛.
struct Item {
    long long pos;
    long long tastiness;
};

Item item[MAXITEM];
long long gain[MAXGAIN];

bool CmpByPos(const Item &a, const Item &b) {
    return a.pos < b.pos;
}

int main() {
    int K, M, N;
    scanf("%d%d%d", &K, &M, &N);
    for (int i = 0; i < K; ++i) {
        scanf("%lld%lld", &item[i].pos, &item[i].tastiness);
    }
    for (int i = K; i < K + M; ++i) {
        scanf("%lld", &item[i].pos);
        item[i].tastiness = -1;
    }
    sort(item, item + K + M, CmpByPos);

    int total = K + M;
    int gainCount = 0;
    int lastCow = -1;            // 上一个遇到的 Nhoj 牛的下标
    long long segmentTastiness = 0;  // 两个 Nhoj 牛之间还没结算的草块美味值之和

    for (int i = 0; i < total; ++i) {
        if (item[i].tastiness >= 0) {
            segmentTastiness += item[i].tastiness;
        } else {
            if (lastCow == -1) {
                // 最左端这一段: 把一头牛放在最左的 Nhoj 牛左边一点, 整段全归 FJ.
                gain[gainCount++] = segmentTastiness;
            } else {
                // 左右两端都是 Nhoj 牛的段: 一头牛能拿的是长度固定为
                // (右牛位置 - 左牛位置) / 2 的窗口, 窗口沿数轴滑动,
                // 所以用双指针扫出窗口和的最大值.
                long long span = item[i].pos - item[lastCow].pos;
                long long window = 0;  // 当前窗口里的美味值之和
                long long best = 0;    // 所有窗口里的最大值, 即一头牛的收益
                int r = lastCow;       // 窗口右端, 指向最靠右被圈进来的草块
                for (int j = lastCow + 1; j < i; ++j) {
                    // 让窗口左端贴着第 j 块草的左侧; 只要跨度还没到 span/2,
                    // 后面的草块就同样落在窗口内. 乘 2 避免除以 2 的精度问题.
                    while (r + 1 < i && 2 * (item[r + 1].pos - item[j].pos) < span) {
                        window += item[++r].tastiness;
                    }
                    if (window > best) best = window;
                    window -= item[j].tastiness;  // 左端越过第 j 块草
                }
                // 两头牛一定够把整段拿完: 一头贴着左牛, 一头贴着右牛.
                gain[gainCount++] = best;
                gain[gainCount++] = segmentTastiness - best;
            }
            lastCow = i;
            segmentTastiness = 0;
        }
    }
    gain[gainCount++] = segmentTastiness;  // 最右端这一段, 一头牛即可全拿

    // 增量从大到小取前 N 个. 段内"第二头牛的额外收益"不会超过"第一头牛的收益",
    // 所以倒序取前 N 个时, 一旦取到某段的额外收益, 该段的一牛收益也一定在里面,
    // 选出的增量总能对应到合法的放牛方案; 多出来的名额贡献 0, 不影响答案.
    sort(gain, gain + gainCount);
    long long answer = 0;
    for (int i = 0; i < N && i < gainCount; ++i) {
        answer += gain[gainCount - 1 - i];
    }
    printf("%lld\n", answer);
    return 0;
}
