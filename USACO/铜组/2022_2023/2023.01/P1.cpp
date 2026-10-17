#include <cstdio>
#include <string>
#include <vector>

using namespace std;

int main() {
    int n;
    char buffer[100005];
    scanf("%d %s", &n, buffer);
    string s = buffer;

    vector<int> e(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &e[i]);
        --e[i];  // 转换为 0-based
    }

    // 找到两种牛第一次出现和最后一次出现的位置
    int firstG = -1, lastG = -1;
    int firstH = -1, lastH = -1;
    for (int i = 0; i < n; ++i) {
        if (s[i] == 'G') {
            if (firstG == -1) firstG = i;
            lastG = i;
        } else {
            if (firstH == -1) firstH = i;
            lastH = i;
        }
    }

    // 领导者的必要条件:
    // 如果某种牛的领导者是通过"包含本品种所有牛"当选的, 那必须是第一头出现的该牛, 且 e[first] >= last
    bool gLeaderCanCoverAll = (e[firstG] >= lastG);
    bool hLeaderCanCoverAll = (e[firstH] >= lastH);

    long long pairs = 0;

    // 情况 1: G 的领导者包含所有 G, H 的领导者包含所有 H (互不依赖)
    if (gLeaderCanCoverAll && hLeaderCanCoverAll) {
        // 如果 firstG 的名单没有包含 firstH, 且 firstH 的名单没有包含 firstG, 它们直接可以作为一对
        // 事实上, 只要各自能包含全部同种牛, 无论是否包含对方, 都满足条件!
        ++pairs;
    }

    // 情况 2: G 领导者包含全部 G (即 firstG), 寻找位于 firstG 之前的 H 牛, 其名单包含了 firstG
    if (gLeaderCanCoverAll) {
        for (int i = 0; i < firstG; ++i) {
            if (s[i] == 'H' && e[i] >= firstG) {
                // 注意如果 i 就是 firstH 且 hLeaderCanCoverAll, 已经在情况 1 计入了, 避免重复
                if (!(i == firstH && hLeaderCanCoverAll)) {
                    ++pairs;
                }
            }
        }
    }

    // 情况 3: H 领导者包含全部 H (即 firstH), 寻找位于 firstH 之前的 G 牛, 其名单包含了 firstH
    if (hLeaderCanCoverAll) {
        for (int i = 0; i < firstH; ++i) {
            if (s[i] == 'G' && e[i] >= firstH) {
                if (!(i == firstG && gLeaderCanCoverAll)) {
                    ++pairs;
                }
            }
        }
    }

    printf("%lld\n", pairs);
    return 0;
}
