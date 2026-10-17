#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXHALF = 1 << 20;  // N <= 40, 每半最多 20 条指令, 子集数上限 2^20

struct Step {  // 一条指令
    long long x;
    long long y;
};

struct Subset {  // 某个子集的和, cnt 是选了几条指令
    long long x;
    long long y;
    int cnt;
};

Step dirs[45];
Subset leftHalf[MAXHALF];
Subset rightHalf[MAXHALF];
long long answer[45];

bool CmpSubset(const Subset &a, const Subset &b) {
    if (a.x != b.x) return a.x < b.x;
    if (a.y != b.y) return a.y < b.y;
    return a.cnt < b.cnt;
}

// 两个辅助函数在 rightHalf 中做二分: 找 (x,y) 的字典序下界/上界
bool KeyLess(long long px, long long py, long long keyX, long long keyY) {
    if (px != keyX) return px < keyX;
    return py < keyY;
}

int LowerKey(long long keyX, long long keyY, int cnt) {
    int lo = 0, hi = cnt;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (KeyLess(rightHalf[mid].x, rightHalf[mid].y, keyX, keyY)) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

int UpperKey(long long keyX, long long keyY, int cnt) {
    int lo = 0, hi = cnt;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (KeyLess(keyX, keyY, rightHalf[mid].x, rightHalf[mid].y)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

// 用倍增的方式枚举前 n 条指令的所有子集: 每加一条指令, 子集数翻倍
void Enumerate(const Step *steps, int n, Subset *out, int &outCnt) {
    out[0].x = 0;
    out[0].y = 0;
    out[0].cnt = 0;
    outCnt = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < outCnt; ++j) {
            out[outCnt + j].x = out[j].x + steps[i].x;
            out[outCnt + j].y = out[j].y + steps[i].y;
            out[outCnt + j].cnt = out[j].cnt + 1;
        }
        outCnt *= 2;
    }
}

int main() {
    int n;
    long long goalX, goalY;
    scanf("%d", &n);
    scanf("%lld%lld", &goalX, &goalY);
    for (int i = 0; i < n; ++i) {
        scanf("%lld%lld", &dirs[i].x, &dirs[i].y);
    }

    // 折半枚举: 前一半的每个子集给出位移 offset, 后一半需要补上 goal - offset.
    // 两边都排序后, 对每个 offset 用二分圈出后一半中位移恰好相等的区间,
    // 在这个区间里统计每种"指令条数"各有多少个.
    int nLeft = n / 2, nRight = n - nLeft;
    int cntLeft, cntRight;
    Enumerate(dirs, nLeft, leftHalf, cntLeft);
    Enumerate(dirs + nLeft, nRight, rightHalf, cntRight);
    sort(leftHalf, leftHalf + cntLeft, CmpSubset);
    sort(rightHalf, rightHalf + cntRight, CmpSubset);

    int hist[45];
    for (int l = 0; l < cntLeft;) {
        int r = l;
        while (r < cntLeft && leftHalf[r].x == leftHalf[l].x &&
               leftHalf[r].y == leftHalf[l].y) {
            ++r;
        }
        long long needX = goalX - leftHalf[l].x;
        long long needY = goalY - leftHalf[l].y;
        int lo = LowerKey(needX, needY, cntRight);
        int hi = UpperKey(needX, needY, cntRight);
        for (int c = 0; c <= nRight; ++c) hist[c] = 0;
        for (int t = lo; t < hi; ++t) ++hist[rightHalf[t].cnt];
        for (int t = l; t < r; ++t) {
            int used = leftHalf[t].cnt;
            for (int c = 0; c <= nRight; ++c) {
                if (hist[c] != 0) answer[used + c] += hist[c];
            }
        }
        l = r;
    }

    for (int k = 1; k <= n; ++k) printf("%lld\n", answer[k]);
    return 0;
}
