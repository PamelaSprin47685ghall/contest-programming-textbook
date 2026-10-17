#include <algorithm>
#include <cstdio>
#include <cstring>

using namespace std;

const int MAXN = 100005;  // 牛的数量上限
const int MAXL = 200005;  // 拼接串长度上限：名字总长 10^5，再加上每头牛一个分隔符
const int MAXLOG = 18;    // 2^18 > 2×10^5，稀疏表的层数

int n;                 // 牛的数量
int lenT;              // 拼接串的总长度
char text[MAXL];       // 所有名字拼成的长串，名字之间插入分隔符 '\1'
char nameBuf[MAXL];    // 读入单个名字用的缓冲
int nameLen[MAXN];     // 每头牛名字的长度
int owner[MAXL];       // 每个位置属于哪头牛；分隔符位置记为 -1
int suffixLen[MAXL];   // 从该位置开始的后缀在自己名字内的长度

int sa[MAXL], rk[MAXL], tmpSA[MAXL], tmpRK[MAXL], bucket[MAXL];
int rankPos[MAXL];  // rankPos[i]：后缀 i 在 sa 中的下标
int lcpOf[MAXL];    // lcpOf[i]：sa[i] 与 sa[i+1] 的最长公共前缀长度
int sparse[MAXLOG][MAXL];  // sparse[t][i]：lcpOf[i .. i+2^t-1] 的最小值
int leftShare[MAXL];       // 与 SA 中左边最近的“别的牛”的后缀的最长公共前缀
int rightShare[MAXL];      // 与 SA 中右边最近的“别的牛”的后缀的最长公共前缀
long long answer[MAXN];
int lastEnd[MAXN];  // 同一头牛在 SA 中上一段连续后缀的结束下标，-1 表示还没有

// 倍增法 + 计数排序构造后缀数组
void buildSuffixArray() {
    for (int i = 0; i < 256; ++i) bucket[i] = 0;
    for (int i = 0; i < lenT; ++i) ++bucket[(unsigned char)text[i]];
    for (int i = 1; i < 256; ++i) bucket[i] += bucket[i - 1];
    for (int i = lenT - 1; i >= 0; --i) sa[--bucket[(unsigned char)text[i]]] = i;
    rk[sa[0]] = 0;
    int classes = 1;
    for (int i = 1; i < lenT; ++i) {
        if (text[sa[i]] != text[sa[i - 1]]) ++classes;
        rk[sa[i]] = classes - 1;
    }
    for (int k = 1; k < lenT && classes < lenT; k <<= 1) {
        // 先按第二关键字排序：位置 ≥ k 的后缀整体前移，末尾 k 个后缀的第二关键字最小
        int p = 0;
        for (int i = lenT - k; i < lenT; ++i) tmpSA[p++] = i;
        for (int i = 0; i < lenT; ++i)
            if (sa[i] >= k) tmpSA[p++] = sa[i] - k;
        // 再按第一关键字计数排序，这样得到的顺序对第一关键字稳定
        for (int i = 0; i < classes; ++i) bucket[i] = 0;
        for (int i = 0; i < lenT; ++i) ++bucket[rk[i]];
        for (int i = 1; i < classes; ++i) bucket[i] += bucket[i - 1];
        for (int i = lenT - 1; i >= 0; --i) {
            int x = tmpSA[i];
            sa[--bucket[rk[x]]] = x;
        }
        tmpRK[sa[0]] = 0;
        int newClasses = 1;
        for (int i = 1; i < lenT; ++i) {
            int a = sa[i], b = sa[i - 1];
            int a2 = (a + k < lenT) ? rk[a + k] : -1;
            int b2 = (b + k < lenT) ? rk[b + k] : -1;
            if (rk[a] != rk[b] || a2 != b2) ++newClasses;
            tmpRK[a] = newClasses - 1;
        }
        for (int i = 0; i < lenT; ++i) rk[i] = tmpRK[i];
        classes = newClasses;
    }
}

// Kasai 算法：用已经算出的相邻位置答案推出当前位置，整体线性
void buildLCP() {
    for (int i = 0; i < lenT; ++i) rankPos[sa[i]] = i;
    int h = 0;
    for (int i = 0; i < lenT; ++i) {
        if (rankPos[i] > 0) {
            int j = sa[rankPos[i] - 1];
            while (i + h < lenT && j + h < lenT && text[i + h] == text[j + h]) ++h;
            lcpOf[rankPos[i] - 1] = h;
            if (h > 0) --h;
        } else {
            h = 0;
        }
    }
    lcpOf[lenT - 1] = 0;  // 最后一个后缀右边没有后缀
}

// 区间最小值查询用的稀疏表
void buildSparse() {
    for (int i = 0; i < lenT; ++i) sparse[0][i] = lcpOf[i];
    for (int t = 1; t < MAXLOG; ++t) {
        int half = 1 << (t - 1);
        for (int i = 0; i + (1 << t) <= lenT; ++i)
            sparse[t][i] = min(sparse[t - 1][i], sparse[t - 1][i + half]);
    }
}

// lcpOf 上闭区间 [l, r] 的最小值
int rangeMin(int l, int r) {
    int t = 0;
    while ((1 << (t + 1)) <= r - l + 1) ++t;
    return min(sparse[t][l], sparse[t][r - (1 << t) + 1]);
}

int main() {
    scanf("%d", &n);
    lenT = 0;
    for (int i = 0; i < n; ++i) {
        scanf("%s", nameBuf);
        int L = (int)strlen(nameBuf);
        nameLen[i] = L;
        for (int j = 0; j < L; ++j) {
            text[lenT] = nameBuf[j];
            owner[lenT] = i;
            suffixLen[lenT] = L - j;
            ++lenT;
        }
        // 分隔符取 '\1'：比 'a' 小，于是所有以分隔符开头的后缀都排在 SA 最前面
        text[lenT] = 1;
        owner[lenT] = -1;
        suffixLen[lenT] = 0;
        ++lenT;
    }
    buildSuffixArray();
    buildLCP();
    buildSparse();

    // SA 中前 n 个后缀都以分隔符开头，真正属于某头牛的后缀从下标 n 开始
    int firstReal = n;
    for (int i = 0; i < n; ++i) lastEnd[i] = -1;
    for (int l = firstReal; l < lenT;) {
        int id = owner[sa[l]];
        int r = l;
        while (r + 1 < lenT && owner[sa[r + 1]] == id) ++r;
        // 块是极大的，所以左右两侧最近的后缀一定来自别的牛
        int curRight = (r + 1 < lenT) ? lcpOf[r] : 0;
        for (int k = r; k >= l; --k) {
            rightShare[k] = curRight;
            if (k > l && lcpOf[k - 1] < curRight) curRight = lcpOf[k - 1];
        }
        int curLeft = (l == firstReal) ? 0 : lcpOf[l - 1];
        for (int k = l; k <= r; ++k) {
            leftShare[k] = curLeft;
            int prevSame;  // 与同一头牛的上一个后缀（在 SA 中紧邻其前）的最长公共前缀
            if (k > l) {
                prevSame = lcpOf[k - 1];
            } else if (lastEnd[id] < 0) {
                prevSame = 0;
            } else {
                prevSame = rangeMin(lastEnd[id], l - 1);
            }
            int shared = max(prevSame, max(leftShare[k], rightShare[k]));
            if (suffixLen[sa[k]] > shared) answer[id] += suffixLen[sa[k]] - shared;
            if (k < r && lcpOf[k] < curLeft) curLeft = lcpOf[k];
        }
        lastEnd[id] = r;
        l = r + 1;
    }
    for (int i = 0; i < n; ++i) printf("%lld\n", answer[i]);
    return 0;
}
