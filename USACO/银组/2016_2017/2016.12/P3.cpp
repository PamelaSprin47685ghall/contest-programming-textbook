#include <cstdio>

using namespace std;

const int MAXN = 205;

int cowX[MAXN], cowY[MAXN], cowPower[MAXN];
bool reached[MAXN];
int n;

// 从奶牛 cow 开始, 沿着"能直接收到"的有向边一路标记, 返回能到达的奶牛数.
int ReachFrom(int cow) {
    reached[cow] = true;
    int total = 1;
    for (int next = 0; next < n; ++next) {
        if (reached[next]) continue;
        // 比较两点的平方距离与功率的平方, 避免开根号带来浮点误差.
        long long dx = cowX[cow] - cowX[next];
        long long dy = cowY[cow] - cowY[next];
        long long limit = (long long)cowPower[cow] * cowPower[cow];
        if (dx * dx + dy * dy <= limit) {
            total += ReachFrom(next);
        }
    }
    return total;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d%d%d", &cowX[i], &cowY[i], &cowPower[i]);
    }

    int best = 1;  // 广播者自己一定收得到, 所以至少有 1 头
    for (int start = 0; start < n; ++start) {
        for (int i = 0; i < n; ++i) {
            reached[i] = false;
        }
        int count = ReachFrom(start);
        if (count > best) best = count;
    }
    printf("%d\n", best);
    return 0;
}
