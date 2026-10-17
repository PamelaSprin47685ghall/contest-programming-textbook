#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 20;
const int MAXE = 60000;  // 3^10 = 59049, 是半边 (取值, 集合) 对的数量上限

int n, n1, n2;
long long milk[MAXN];
long long milkA[10], milkB[10];

struct Entry {
    long long value;  // 这半边内部的不平衡量, 只保留非负的一侧
    int mask;         // 这半边用到的奶牛集合

    bool operator<(const Entry &other) const {
        if (value != other.value) return value < other.value;
        return mask < other.mask;
    }
    bool operator==(const Entry &other) const {
        return value == other.value && mask == other.mask;
    }
};

Entry listA[MAXE], listB[MAXE];
int cntA, cntB;
bool good[1 << MAXN];  // good[S]: 全集中的集合 S 是不是平衡的

// 枚举半边的所有方案: 每头牛有三种状态——不选 / 放到第一组 / 放到第二组
void enumerate(long long *half, int k, Entry *out, int &cnt) {
    int total = 1;
    for (int i = 0; i < k; i++) total *= 3;
    cnt = 0;
    for (int code = 0; code < total; code++) {
        int t = code;
        long long value = 0;
        int mask = 0;
        for (int i = 0; i < k; i++) {
            int d = t % 3;
            t /= 3;
            if (d == 1) {
                value += half[i];
                mask |= 1 << i;
            } else if (d == 2) {
                value -= half[i];
                mask |= 1 << i;
            }
        }
        // 两组对调就能把 -value 变成 value, 所以只需留下非负的一侧
        if (value >= 0) {
            out[cnt].value = value;
            out[cnt].mask = mask;
            cnt++;
        }
    }
    sort(out, out + cnt);
    cnt = unique(out, out + cnt) - out;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%lld", &milk[i]);
    n1 = n / 2;
    n2 = n - n1;
    for (int i = 0; i < n1; i++) milkA[i] = milk[i];
    for (int i = 0; i < n2; i++) milkB[i] = milk[n1 + i];

    enumerate(milkA, n1, listA, cntA);
    enumerate(milkB, n2, listB, cntB);

    // 两边不平衡量相同的方案拼起来就是一个平衡子集
    int p = 0, q = 0;
    while (p < cntA && q < cntB) {
        if (listA[p].value < listB[q].value) {
            p++;
        } else if (listA[p].value > listB[q].value) {
            q++;
        } else {
            int p2 = p, q2 = q;
            while (p2 < cntA && listA[p2].value == listA[p].value) p2++;
            while (q2 < cntB && listB[q2].value == listB[q].value) q2++;
            for (int i = p; i < p2; i++) {
                for (int j = q; j < q2; j++) {
                    good[listA[i].mask | (listB[j].mask << n1)] = true;
                }
            }
            p = p2;
            q = q2;
        }
    }

    int ans = 0;
    for (int mask = 1; mask < (1 << n); mask++) {
        if (good[mask]) ans++;
    }
    printf("%d\n", ans);
    return 0;
}
