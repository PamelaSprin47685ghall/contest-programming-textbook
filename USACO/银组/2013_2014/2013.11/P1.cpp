#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>

using namespace std;

const int MAXN = 105;   // 清单最多 100 行
const int MAXA = 30;    // 每行最多 30 个形容词
const int MAXW = 20;    // 每个单词不超过 10 个字母，留出余量

int n;
long long k;

string words[MAXN][MAXA];   // 清单上第 i 行的第 j 个形容词
int wcnt[MAXN];             // 每行的形容词个数（各行相同）

int npos;                   // 形容词的层数
string adj[MAXA][MAXN];     // 每一层出现过的形容词
int cnt[MAXA];              // 每一层不同形容词的个数

long long blocked[MAXN];    // 清单上每头牛在“字典序组合”中的编号
int nb;

// 二分查找 word 在某一层中的排名：word 必然出现在这一层里
int rankOf(int pos, const string &word) {
    int lo = 0, hi = cnt[pos] - 1;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (adj[pos][mid] < word) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

int main() {
    scanf("%d %lld", &n, &k);
    char buf[MAXW];
    for (int i = 0; i < n; i++) {
        // 每句话开头固定是四个单词 "Farmer John has no"，直接跳过
        for (int t = 0; t < 4; t++) {
            scanf("%s", buf);
        }
        wcnt[i] = 0;
        while (true) {
            scanf("%s", buf);
            if (strcmp(buf, "cow.") == 0) {
                break;
            }
            words[i][wcnt[i]] = string(buf);
            wcnt[i]++;
        }
        npos = wcnt[i];
    }

    // 每一层的候选形容词就是清单里该层出现过的词，去重后排序；
    // 总量只有 N*30，线性去重即可
    for (int i = 0; i < n; i++) {
        for (int p = 0; p < npos; p++) {
            bool seen = false;
            for (int t = 0; t < cnt[p]; t++) {
                if (adj[p][t] == words[i][p]) {
                    seen = true;
                    break;
                }
            }
            if (!seen) {
                adj[p][cnt[p]] = words[i][p];
                cnt[p]++;
            }
        }
    }
    for (int p = 0; p < npos; p++) {
        sort(adj[p], adj[p] + cnt[p]);
    }

    // 把每头牛编码成“最后一层变化最快”的混合进制数：
    // 这个编号的大小顺序与逐词比较的字典序完全一致
    for (int i = 0; i < n; i++) {
        long long idx = 0;
        for (int p = 0; p < npos; p++) {
            idx = idx * cnt[p] + rankOf(p, words[i][p]);
        }
        blocked[nb] = idx;
        nb++;
    }
    sort(blocked, blocked + nb);

    // 求第 K 头“拥有”的牛：编号 <= idx 的组合里拥有的个数为
    // (idx+1) - (编号 <= idx 的缺失牛数)，它应当等于 K。
    // 先设 idx = K-1，每遇到一头编号不超过 idx 的缺失牛就把它跳过，
    // 答案编号顺延一位；这种顺延最多 N 次
    long long idx = k - 1;
    int ptr = 0;
    while (ptr < nb && blocked[ptr] <= idx) {
        idx++;
        ptr++;
    }

    // 把编号解码回每一层的排名，再输出对应的形容词
    int pick[MAXA];
    long long rest = idx;
    for (int p = npos - 1; p >= 0; p--) {
        pick[p] = (int)(rest % cnt[p]);
        rest /= cnt[p];
    }
    for (int p = 0; p < npos; p++) {
        if (p > 0) {
            printf(" ");
        }
        printf("%s", adj[p][pick[p]].c_str());
    }
    printf("\n");
    return 0;
}
