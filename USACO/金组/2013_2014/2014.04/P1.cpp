#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100006;
const int BREEDS = 8;         // 品种编号只有 1..8
const int KEYLEN = 9;         // 签名 = 8 个品种的计数 + 品种集合的掩码
const int TABLE = 1 << 18;    // 开放寻址哈希表容量

struct Cow {
    int position;
    int breed;
};

int n, k;
Cow cow[MAXN];
int prefixCount[MAXN][BREEDS];            // 前 p 头牛里每个品种的数量
unsigned char backMask[MAXN * BREEDS];     // 以第 i 头牛结尾时，最近的 j+1 个不同品种
unsigned char forwardMask[MAXN * BREEDS];  // 从第 i 头牛出发时，最近的 j+1 个不同品种
int breedCount[1 << BREEDS];              // 每个掩码里有几个品种

int tablePos[TABLE];                      // 签名 -> 最靠左的左端点位置
int tableKey[TABLE][KEYLEN];              // 保存完整签名，哈希冲突时精确比较

bool cowOrder(const Cow& a, const Cow& b) {
    return a.position < b.position;
}

// 前缀 p 处、品种集合 mask 的签名：mask 内的品种都减去其中第一个品种的数量，
// 于是签名相等就等价于「mask 内各品种同增、mask 外各品种不变」。
void buildKey(int p, int mask, int key[]) {
    int base = 0;
    for (int b = 0; b < BREEDS; b++) {
        if (mask >> b & 1) {
            base = prefixCount[p][b];
            break;
        }
    }
    for (int b = 0; b < BREEDS; b++) {
        if (mask >> b & 1) key[b] = prefixCount[p][b] - base;
        else key[b] = prefixCount[p][b];
    }
    key[BREEDS] = mask;
}

unsigned long long hashKey(const int key[]) {
    unsigned long long h = 1469598103934665603ULL;
    for (int t = 0; t < KEYLEN; t++) {
        h ^= (unsigned int)key[t];
        h *= 1099511628211ULL;
    }
    return h;
}

bool sameKey(int slot, const int key[]) {
    for (int t = 0; t < KEYLEN; t++) {
        if (tableKey[slot][t] != key[t]) return false;
    }
    return true;
}

// 返回签名所在的槽；allowInsert 为真且签名不存在时把它放进空槽。
int findSlot(const int key[], unsigned long long h, bool allowInsert) {
    int slot = (int)(h & (TABLE - 1));
    while (tablePos[slot] != -1) {
        if (sameKey(slot, key)) return slot;
        slot = (slot + 1) & (TABLE - 1);
    }
    if (!allowInsert) return -1;
    for (int t = 0; t < KEYLEN; t++) tableKey[slot][t] = key[t];
    return slot;
}

int main() {
    scanf("%d %d", &n, &k);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &cow[i].position, &cow[i].breed);
    }
    sort(cow, cow + n, cowOrder);
    for (int i = 0; i < n; i++) {
        cow[i].breed--;  // 品种改成 0 基，方便当位掩码用
    }

    for (int mask = 0; mask < (1 << BREEDS); mask++) {
        int cnt = 0;
        for (int b = 0; b < BREEDS; b++) {
            if (mask >> b & 1) cnt++;
        }
        breedCount[mask] = cnt;
    }
    for (int b = 0; b < BREEDS; b++) {
        prefixCount[0][b] = 0;
    }
    for (int i = 0; i < n; i++) {
        for (int b = 0; b < BREEDS; b++) {
            prefixCount[i + 1][b] = prefixCount[i][b];
        }
        prefixCount[i + 1][cow[i].breed]++;
    }

    // 照片的品种集合随着左端点左移只会变大，所以固定右端点时可能的集合
    // 就是「最近 j+1 个不同品种」这 8 种；下面先把它们算出来。
    if (n < 1) return 0;
    int firstBreed = 1 << cow[0].breed;
    int lastBreed = 1 << cow[n - 1].breed;
    for (int j = 0; j < BREEDS; j++) {
        backMask[j] = firstBreed;
        forwardMask[(n - 1) * BREEDS + j] = lastBreed;
    }
    for (int i = 1; i < n; i++) {
        int bit = 1 << cow[i].breed;
        backMask[i * BREEDS] = bit;
        for (int j = 1; j < BREEDS; j++) {
            if (backMask[(i - 1) * BREEDS + j] & bit) backMask[i * BREEDS + j] = backMask[(i - 1) * BREEDS + j];
            else backMask[i * BREEDS + j] = bit | backMask[(i - 1) * BREEDS + j - 1];
        }
    }
    for (int i = n - 2; i >= 0; i--) {
        int bit = 1 << cow[i].breed;
        forwardMask[i * BREEDS] = bit;
        for (int j = 1; j < BREEDS; j++) {
            if (forwardMask[(i + 1) * BREEDS + j] & bit) forwardMask[i * BREEDS + j] = forwardMask[(i + 1) * BREEDS + j];
            else forwardMask[i * BREEDS + j] = bit | forwardMask[(i + 1) * BREEDS + j - 1];
        }
    }

    int best = -1;
    int key[KEYLEN];
    for (int j = k - 1; j < BREEDS; j++) {
        int want = j + 1;
        for (int t = 0; t < TABLE; t++) {
            tablePos[t] = -1;
        }
        // 左端点：签名取第 i 头牛之前的前缀，位置就是第 i 头牛的位置。
        // 从右往左扫，同一签名最后写入的就是最靠左的起点。
        for (int i = n - 1; i >= 0; i--) {
            int mask = forwardMask[i * BREEDS + j];
            if (breedCount[mask] != want) continue;
            buildKey(i, mask, key);
            int slot = findSlot(key, hashKey(key), true);
            tablePos[slot] = cow[i].position;
        }
        // 右端点：签名取含第 i 头牛的前缀，两者相等说明区间内各品种数量
        // 恰好同增，而且左端点那头牛的品种一定属于 mask，所以共同增量至少为 1。
        for (int i = 0; i < n; i++) {
            int mask = backMask[i * BREEDS + j];
            if (breedCount[mask] != want) continue;
            buildKey(i + 1, mask, key);
            int slot = findSlot(key, hashKey(key), false);
            if (slot == -1) continue;
            if (tablePos[slot] < cow[i].position) {
                int size = cow[i].position - tablePos[slot];
                if (size > best) best = size;
            }
        }
    }
    printf("%d\n", best);
    return 0;
}
