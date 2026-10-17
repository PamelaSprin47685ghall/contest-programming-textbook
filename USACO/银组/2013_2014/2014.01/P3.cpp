#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 155;

struct Program {
    int startTime;
    int endTime;
};

Program programs[MAXN];

bool ByEndTime(const Program& a, const Program& b) { return a.endTime < b.endTime; }

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d%d", &programs[i].startTime, &programs[i].endTime);
    }
    // 结束得早的节目先考虑: 越早腾出磁带, 后面能塞的节目越多
    sort(programs, programs + n, ByEndTime);

    // last1 <= last2: 两台磁带各自"最后录制的节目"的结束时刻, 时刻本身不重要, 重要的是磁带什么时候重新空出来
    int last1 = 0, last2 = 0;
    int recorded = 0;
    for (int i = 0; i < n; ++i) {
        int s = programs[i].startTime, e = programs[i].endTime;
        if (s < last1) {
            // last1 <= last2, 连空得更早的那台都在忙, 这个节目两台磁带都录不了
            continue;
        }
        if (s < last2) {
            // 只有空得更早的那台来得及录它
            last1 = e;
        } else {
            // 两台都来得及: 占用空得更晚的那台, 把空得更早的那台留给后面的节目
            last2 = e;
        }
        ++recorded;
        if (last1 > last2) swap(last1, last2);  // 维护 last1 <= last2 这个前提
    }
    printf("%d\n", recorded);
    return 0;
}
