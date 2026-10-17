#include <cstdio>
#include <queue>

using namespace std;

const int MAXN = 200005;

struct Event {
    long long time;
    int a;
    int b;
    // 优先队列默认大根堆，把比较反过来让相遇时刻最小的先出队
    bool operator<(const Event &other) const {
        return time > other.time;
    }
};

int n;
long long p[MAXN];
long long s[MAXN];
long long answer[MAXN];

// 链表：还存活着的、紧挨在左右两侧的粒子
int leftNode[MAXN];
int rightNode[MAXN];

// 相邻存活粒子 a（左）与 b（右）会在第几次观察时被判为消失
long long meetTime(int a, int b) {
    long long dist = p[b] - p[a];
    long long speedSum = s[a] + s[b];
    long long rounds = (dist + speedSum - 1) / speedSum;
    // 下标为偶数是哞微子（初始向右），为奇数是反哞微子（初始向左）。
    // 第 k 次观察的时刻是 T_k = k(k+1)/2，每次观察时所有粒子一起反向。
    // 由运动规律可得：T_k 时偶下标粒子的位移是 (-1)^(k+1) * ceil(k/2) * s，
    // 奇下标粒子则相反。于是左右端点在"相向而行"的那些观察上彼此靠近
    // dist 会被每轮抵消 s[a]+s[b]：
    //   a 为偶数时它们在奇数号观察靠近，故答案是 2*rounds-1；
    //   a 为奇数时它们先相互远离，要到偶数号观察才靠近，故答案是 2*rounds。
    return 2 * rounds - (a % 2 == 0 ? 1 : 0);
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 0; i < n; ++i) scanf("%lld", &p[i]);
        for (int i = 0; i < n; ++i) scanf("%lld", &s[i]);

        for (int i = 0; i < n; ++i) {
            answer[i] = 0;   // 0 表示还没消失
            leftNode[i] = i - 1;
            rightNode[i] = i + 1;
        }

        priority_queue<Event> pq;
        for (int i = 1; i < n; ++i) {
            pq.push({meetTime(i - 1, i), i - 1, i});
        }

        while (!pq.empty()) {
            Event cur = pq.top();
            pq.pop();
            int a = cur.a;
            int b = cur.b;
            // 只要有一方已经消失，这条记录就作废。两个都还活着时它们一定仍然相邻：
            // 只有相邻的一对才会消失，中间不可能凭空插进来新的粒子
            if (answer[a] != 0 || answer[b] != 0) continue;
            answer[a] = cur.time;
            answer[b] = cur.time;

            // 这一对消失后，原来隔着它们的左右两个粒子变成相邻，只有这一对新组合需要计算
            int l = leftNode[a];
            int r = rightNode[b];
            if (l >= 0 && r < n) {
                rightNode[l] = r;
                leftNode[r] = l;
                pq.push({meetTime(l, r), l, r});
            }
        }

        for (int i = 0; i < n; ++i) {
            printf("%lld%c", answer[i], i + 1 == n ? '\n' : ' ');
        }
    }
    return 0;
}
