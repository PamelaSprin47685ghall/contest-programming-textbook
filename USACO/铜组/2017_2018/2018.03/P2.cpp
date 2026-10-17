#include <algorithm>
#include <cstdio>
#include <deque>
#include <vector>

using namespace std;

// 阶层顺序: 记录必须按给定次序出现的牛(其余牛可任意穿插).
struct Ordered {
    vector<int> Data;  // 阶层牛的编号(0 基).

    void Read(int m) {
        Data.resize(m);
        for (int i = 0; i < m; ++i) {
            scanf("%d", &Data[i]);
            --Data[i];
        }
    }

    bool Contains0() const {
        for (int cow : Data) {
            if (cow == 0) return true;
        }
        return false;
    }

    // 反转顺序, 便于从后往前构造.
    Ordered& Reverse() {
        reverse(Data.begin(), Data.end());
        return *this;
    }
};

// 一个挤奶顺序安排.
struct Arrange {
    vector<int> Cow;  // Cow[pos] = 该位置的牛编号, -1 表示未定.
    vector<int> At;   // At[cow] = 牛 cow 的位置, -1 表示未定.

    void Read(int n, int k) {
        Cow.assign(n, -1);
        for (int i = 0; i < k; ++i) {
            int cow, pos;
            scanf("%d%d", &cow, &pos);
            Cow[pos - 1] = cow - 1;
        }
    }

    void ComputeAt() {
        At.assign(Cow.size(), -1);
        for (int i = 0; i < (int)Cow.size(); ++i) {
            if (Cow[i] != -1) At[Cow[i]] = i;
        }
    }

    // 把阶层牛按顺序插入空位. current 记录下一个可放的位置.
    Arrange& InsertOrdered(const Ordered& ordered) {
        int current = 0;
        for (int cow : ordered.Data) {
            if (At[cow] == -1) {
                // 该阶层牛未固定, 放到下一个空位.
                while (Cow[current] != -1) ++current;
                Cow[current] = cow;
            } else {
                // 已固定, 后续阶层牛只能放在它之后.
                current = At[cow] + 1;
            }
        }
        return *this;
    }

    Arrange& Reverse() {
        reverse(Cow.begin(), Cow.end());
        return *this;
    }

    int Find(int value) const {
        for (int i = 0; i < (int)Cow.size(); ++i) {
            if (Cow[i] == value) return i;
        }
        return -1;
    }
};

int Compute() {
    int n, m, k;
    scanf("%d%d%d", &n, &m, &k);
    Ordered ordered;
    ordered.Read(m);
    Arrange arrange;
    arrange.Read(n, k);
    arrange.ComputeAt();
    // 1 号牛位置已固定, 直接返回.
    if (arrange.At[0] != -1) return arrange.At[0];
    // 1 号牛在阶层顺序里: 从前往后尽量把阶层牛往前放.
    if (ordered.Contains0()) return arrange.InsertOrdered(ordered).Find(0);
    // 否则 1 号牛是普通牛: 从后往前尽量把阶层牛往后放,
    // 这样 1 号牛能占据最靠前的位置. 反转后按同一逻辑构造再转回.
    arrange.Reverse().ComputeAt();
    // 转回后第一个空位就是 1 号牛该放的位置.
    return arrange.InsertOrdered(ordered.Reverse()).Reverse().Find(-1);
}

int main() {
    printf("%d\n", Compute() + 1);
    return 0;
}