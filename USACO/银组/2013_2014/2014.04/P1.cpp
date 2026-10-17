#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100005;

struct Cow {
    int position;  // 奶牛在篱笆上的位置
    char color;    // 'W' 白牛, 'S' 花牛
};

Cow cows[MAXN];

// 白牛记 +1、花牛记 -1 之后, prefixSum[i] 是前 i 头牛的分数和.
// 照片取 cows[left+1..right] 时, 涂色能让白牛花牛一样多 等价于
// prefixSum[right] - prefixSum[left] >= 0 且 (right - left) 是偶数.
int prefixSum[MAXN];

// 同一个奇偶类别里, 下标越大, 对应的位置越靠右, 照片左端就越靠右, 照片越小.
// 所以只有"前缀和刷新了最小值"的下标才可能成为最优照片的左端点:
// 其余下标都能被一个更靠左、前缀和也不更大的下标取代.
// 阶梯上下标递增、前缀和严格递减, 因此可以二分查找.
int stairSum[2][MAXN];
int stairPos[2][MAXN];  // 该左端点对应的照片最左边位置
int stairSize[2];

bool CompareCow(const Cow &a, const Cow &b) {
    return a.position < b.position;
}

// 把前缀下标 index 加入它所属的奇偶类别
void AddStair(int index) {
    int parity = index % 2;
    int size = stairSize[parity];
    // 阶梯末尾的前缀和最小; 若它不比新下标大, 新下标就被完全取代了
    if (size > 0 && stairSum[parity][size - 1] <= prefixSum[index]) {
        return;
    }
    stairSum[parity][size] = prefixSum[index];
    stairPos[parity][size] = cows[index + 1].position;
    stairSize[parity] = size + 1;
}

// 在奇偶类别 parity 的阶梯上, 找前缀和不超过 limit 的最靠前一项,
// 返回它对应的照片最左边位置; 找不到返回 -1
int FindLeftmost(int parity, int limit) {
    int lo = 0;
    int hi = stairSize[parity] - 1;
    int found = -1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (stairSum[parity][mid] <= limit) {
            found = mid;
            hi = mid - 1;  // 还要继续找更靠前的
        } else {
            lo = mid + 1;  // 阶梯上前缀和递减, 前半段更大
        }
    }
    if (found < 0) {
        return -1;
    }
    return stairPos[parity][found];
}

int main() {
    int N;
    scanf("%d", &N);
    for (int i = 1; i <= N; i++) {
        int position;
        char color;
        scanf("%d %c", &position, &color);
        cows[i].position = position;
        cows[i].color = color;
    }
    sort(cows + 1, cows + N + 1, CompareCow);

    prefixSum[0] = 0;
    for (int i = 1; i <= N; i++) {
        prefixSum[i] = prefixSum[i - 1] + (cows[i].color == 'W' ? 1 : -1);
    }

    int answer = 0;
    for (int right = 1; right <= N; right++) {
        // 照片的牛数 right - left 必须是偶数, 所以 left 与 right 同奇偶,
        // 于是 right 这一步只可能新解锁 left = right - 2
        int left = right - 2;
        if (left >= 0) {
            AddStair(left);
        }
        int leftPosition = FindLeftmost(right % 2, prefixSum[right]);
        if (leftPosition >= 0) {
            int size = cows[right].position - leftPosition;
            if (size > answer) {
                answer = size;
            }
        }
    }
    printf("%d\n", answer);
    return 0;
}
