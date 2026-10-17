#include <cstdio>
#include <vector>
#include <list>
#include <utility>
using namespace std;

const int MAXN = 262150;   // N <= 262144

int n, A[MAXN];
long long numAtLeast;                       // 当前阈值 v 下，值 >= v 的区间总数
vector<list<pair<int, int> > > ivals;       // ivals[l]：左端点为 l 的段内所有极大区间（左端点递增）

long long sumArith(long long l, long long r) {   // l + (l+1) + ... + r
    return (r + l) * (r - l + 1) / 2;
}

// 计算 L 中这些极大区间一共"覆盖"了多少个区间
long long contrib(const list<pair<int, int> > &L) {
    long long ret = 0;
    for (list<pair<int, int> >::const_iterator it = L.begin(); ; ++it) {
        int x = it->first, y = it->second;
        list<pair<int, int> >::const_iterator nx = it;
        ++nx;
        if (nx == L.end()) {
            ret += sumArith(0, y - x);
            break;
        }
        int nextX = nx->first;
        ret += (long long)(nextX - x) * y - sumArith(x, nextX - 1);
    }
    return ret;
}

// 阈值从 v-1 提到 v：一个段里的极大区间两两合并、数量大致减半
void halve(list<pair<int, int> > &L) {
    if (L.size() <= 1) return;
    numAtLeast += contrib(L);
    int maxSoFar = -1;
    list<pair<int, int> > nL;
    list<pair<int, int> >::iterator it = L.begin();
    for (list<pair<int, int> >::iterator cur = L.begin(); cur != L.end(); ++cur) {
        int x = cur->first, y = cur->second;
        // 找到最后一个左端点 <= y 的极大区间：它给出了合并后新区间的右端点
        while (true) {
            list<pair<int, int> >::iterator nx = it;
            ++nx;
            if (nx == L.end() || nx->first > y) break;
            ++it;
        }
        if (it->second > maxSoFar) {
            maxSoFar = it->second;
            nL.push_back(make_pair(x, maxSoFar));
        }
    }
    L.swap(nL);
    numAtLeast -= contrib(L);
}

int main() {
    scanf("%d", &n);
    vector<vector<int> > withA;                 // withA[v]：值为 v 的所有下标
    for (int i = 0; i < n; i++) {
        scanf("%d", &A[i]);
        while ((int)withA.size() <= A[i]) withA.push_back(vector<int>());
        withA[A[i]].push_back(i);
    }
    ivals.assign(n + 1, list<pair<int, int> >());
    // pre / nxt：把当前的"段"串成双向链表（段的左端点之间相连）
    vector<int> pre(n + 1), nxt(n + 1);
    for (int i = 0; i <= n; i++) pre[i] = nxt[i] = i;
    numAtLeast = (long long)n * (n + 1) / 2;
    vector<int> active;                          // 段内极大区间数 > 1 的段，才需要 halve
    long long ans = 0;
    for (int v = 1; numAtLeast > 0; v++) {
        ans += numAtLeast;                       // 值恰好 >= v 的区间个数：累加到答案（每个区间的值 = 它被计入的次数）
        vector<int> nActive;
        for (size_t j = 0; j < active.size(); j++) {
            int l = active[j];
            halve(ivals[l]);
            if (ivals[l].size() > 1) nActive.push_back(l);
        }
        if (v < (int)withA.size()) {
            for (size_t j = 0; j < withA[v].size(); j++) {
                int i = withA[v][j];
                // 值为 v 的位置在阈值 v-1 的划分里是独立的段，现在它不再是分界点，左右两段合并
                int l = pre[i], r = nxt[i + 1];
                bool shouldAdd = (ivals[l].size() <= 1);
                pre[i] = nxt[i + 1] = -1;
                nxt[l] = r;
                pre[r] = l;
                ivals[l].push_back(make_pair(i, i + 1));   // 新出现的极大区间 [i, i+1)
                --numAtLeast;                              // 区间 [i,i] 的值就是 v，不再算作">= v+1"
                ivals[l].splice(ivals[l].end(), ivals[i + 1]);
                shouldAdd = shouldAdd && (ivals[l].size() > 1);
                if (shouldAdd) nActive.push_back(l);
            }
        }
        active.swap(nActive);
    }
    printf("%lld\n", ans);
    return 0;
}
