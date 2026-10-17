#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 100000; // 奶牛数上限

int n;
vector<int> children[MAXN + 1]; // children[v]：直属上司是 v 的奶牛
long long rating[MAXN + 1];     // 每头奶牛的熟练度（最大 10^9，用 long long 保险）
int parent[MAXN + 1];           // 每头奶牛的直属上司
int preOrder[MAXN + 1];         // 前序遍历得到的奶牛序列
int pos[MAXN + 1];              // 每头奶牛在前序遍历中的下标（从 0 开始）
int subtreeSize[MAXN + 1];      // 子树（含自己）的大小
int ratingOrder[MAXN + 1];      // 奶牛编号，按熟练度从大到小排序
int answer[MAXN + 1];

int bit[MAXN + 2]; // 树状数组：记录"已处理过"的奶牛在前序遍历中的位置

void bitAdd(int x) {
    for (; x <= n; x += x & -x) ++bit[x];
}

int bitSum(int x) {
    int s = 0;
    for (; x > 0; x -= x & -x) s += bit[x];
    return s;
}

// 熟练度更高的排在前面
bool byRating(int x, int y) {
    return rating[x] > rating[y];
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%lld", &rating[i]);
    for (int i = 2; i <= n; ++i) {
        scanf("%d", &parent[i]);
        children[parent[i]].push_back(i);
    }

    // 前序遍历：一个点的所有下属在序列中恰好紧跟它排成一段连续区间。
    // 链状树会让递归深度达到 N，容易爆栈，所以用手写栈迭代实现。
    int stack_[MAXN + 1];
    int top = 0;
    stack_[top++] = 1;
    for (int tick = 0; tick < n; ++tick) {
        int cur = stack_[--top];
        preOrder[tick] = cur;
        pos[cur] = tick;
        // 倒序压栈，保证按编号顺序访问（顺序本身不影响正确性，只是习惯）
        for (int i = (int)children[cur].size() - 1; i >= 0; --i) {
            stack_[top++] = children[cur][i];
        }
    }

    // 子树大小：按前序遍历的逆序累加，儿子的规模总在父亲之前确定
    for (int i = 1; i <= n; ++i) subtreeSize[i] = 1;
    for (int i = n - 1; i >= 1; --i) {
        int v = preOrder[i];
        subtreeSize[parent[v]] += subtreeSize[v];
    }

    for (int i = 1; i <= n; ++i) ratingOrder[i] = i;
    sort(ratingOrder + 1, ratingOrder + n + 1, byRating);

    // 按熟练度从高到低处理：轮到奶牛 v 时，树状数组里只有熟练度更高的奶牛，
    // 于是"v 的子树对应区间内已插入的个数"就是 v 的高熟练度下属数
    for (int i = 1; i <= n; ++i) {
        int v = ratingOrder[i];
        int left = pos[v];                   // v 之前的位置个数
        int right = pos[v] + subtreeSize[v]; // 子树覆盖区间 [pos[v], pos[v]+size-1] 的右端
        answer[v] = bitSum(right) - bitSum(left);
        bitAdd(pos[v] + 1); // 树状数组下标从 1 开始
    }

    for (int i = 1; i <= n; ++i) printf("%d\n", answer[i]);
    return 0;
}
