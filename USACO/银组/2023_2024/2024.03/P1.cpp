#include <cstdio>
#include <queue>

using namespace std;

const int MAXN = 300005;

int n, k;
long long t[MAXN];

struct Farmer {
    long long finish;   // 这位农民当前这场面试结束的时刻
    int id;             // 农民编号（0 起）

    // 小根堆：结束早的优先；同一时刻按编号，让模拟过程唯一确定
    bool operator<(const Farmer &other) const {
        if (finish != other.finish) return finish > other.finish;
        return id > other.id;
    }
};

priority_queue<Farmer> busy;      // 正在面试的农民
int evFarmer[2 * MAXN];           // 依次存放所有事件的农民编号
int evBegin[MAXN], evLen[MAXN];   // 第 i 个事件在 evFarmer 中的区间
int evCount = 0;
char canInterview[MAXN];          // 农民 i 是否可能面试 Bessie

int main() {
    scanf("%d %d", &n, &k);
    for (int i = 0; i < n; ++i) scanf("%lld", &t[i]);

    // 时刻 0：农民 i 开始面试奶牛 i
    for (int i = 0; i < k; ++i) busy.push(Farmer{t[i], i});

    int curCow = k;                 // 下一位等待面试的奶牛
    int total = 0;                  // evFarmer 已用长度
    long long bessieTime = 0;
    int bessieFarmer = 0;
    while (true) {
        long long now = busy.top().finish;
        int begin = total;
        // 同一时刻结束面试的农民构成一个「事件」：他们同时空出来，因此可以互相顶替
        while (!busy.empty() && busy.top().finish == now) {
            evFarmer[total++] = busy.top().id;
            busy.pop();
        }
        int len = total - begin;
        if (len > 1) {              // 只有同时空出多人，才存在互换身份的可能，值得记录
            evBegin[evCount] = begin;
            evLen[evCount] = len;
            ++evCount;
        }
        if (curCow + len > n) {     // 剩下的奶牛不够分给这个事件里的所有农民
            bessieTime = now;       // 空着的那位农民此刻就能开始面试 Bessie
            bessieFarmer = evFarmer[begin];
            break;
        }
        for (int i = begin; i < total; ++i) {
            // 队列里的奶牛按顺序进场，这里随便把下一头交给谁都不影响各时刻的结束时间
            busy.push(Farmer{now + t[curCow], evFarmer[i]});
            ++curCow;
        }
    }

    // 逆着时间回推：若某个事件与「可能面试者集合」有交集，
    // 那么该事件里的农民都能通过互换身份让同事件的那位农民改去面试 Bessie
    canInterview[bessieFarmer] = 1;
    for (int e = evCount - 1; e >= 0; --e) {
        bool hit = false;
        for (int i = 0; i < evLen[e]; ++i)
            if (canInterview[evFarmer[evBegin[e] + i]]) { hit = true; break; }
        if (!hit) continue;
        for (int i = 0; i < evLen[e]; ++i) canInterview[evFarmer[evBegin[e] + i]] = 1;
    }

    printf("%lld\n", bessieTime);
    for (int i = 0; i < k; ++i) putchar(canInterview[i] ? '1' : '0');
    putchar('\n');
    return 0;
}
