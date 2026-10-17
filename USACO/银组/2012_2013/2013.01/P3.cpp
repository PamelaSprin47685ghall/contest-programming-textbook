#include <cstdio>

using namespace std;

const int MAXN = 1000000;   // 奶牛数上限
const int MAXSUM = 250000;  // 所有小组人数之和的上限

// 把每个小组的成员名单首尾相接存在一个数组里, 用起始下标和人数定位
int groupBegin[MAXSUM];
int groupSize[MAXSUM];
int remaining[MAXSUM];  // 小组里还没被邀请的人数
int members[MAXSUM];

// 反向索引: 每头牛所属的小组串成一条链表, 省去 vector<vector<int> > 的额外开销
int cowHead[MAXN + 1];
int entryGroup[MAXSUM];
int entryNext[MAXSUM];

bool invited[MAXN + 1];
int queueCows[MAXN + 1];

int main() {
    int n, g;
    scanf("%d%d", &n, &g);
    for (int i = 1; i <= n; ++i) {
        cowHead[i] = -1;
    }

    int total = 0;  // 已经读入的成员个数
    for (int i = 0; i < g; ++i) {
        int size;
        scanf("%d", &size);
        groupBegin[i] = total;
        groupSize[i] = size;
        remaining[i] = size;
        for (int j = 0; j < size; ++j) {
            int cow;
            scanf("%d", &cow);
            members[total] = cow;
            entryGroup[total] = i;
            entryNext[total] = cowHead[cow];
            cowHead[cow] = total;
            ++total;
        }
    }

    // 小组约束只会因为"有奶牛被邀请"而触发, 所以把新被邀请的奶牛逐个放进队列处理
    invited[1] = true;
    queueCows[0] = 1;
    int invitedCount = 0;
    for (int head = 0, tail = 1; head < tail; ++head) {
        int cow = queueCows[head];
        ++invitedCount;
        for (int e = cowHead[cow]; e != -1; e = entryNext[e]) {
            int grp = entryGroup[e];
            --remaining[grp];
            if (remaining[grp] == 1) {
                // 这一组只剩最后一头奶牛没被邀请, 扫描名单把她找出来.
                // 触发后这头奶牛也被计入, 小组的计数立刻变成 0, 每个小组最多触发一次,
                // 所以扫描的总代价不超过所有小组人数之和
                int end = groupBegin[grp] + groupSize[grp];
                for (int t = groupBegin[grp]; t < end; ++t) {
                    if (!invited[members[t]]) {
                        invited[members[t]] = true;
                        queueCows[tail++] = members[t];
                        break;
                    }
                }
            }
        }
    }

    printf("%d\n", invitedCount);
    return 0;
}
