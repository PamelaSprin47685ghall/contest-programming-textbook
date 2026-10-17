#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 100005;

int n;
int base[MAXN];          // base[v]: 当前所有肥皂盘子栈中, 标签 v 应该放到的那个栈的编号
vector<int> items[MAXN]; // 每个栈内自底向上的盘子标签(自底向上递减)

int main() {
    scanf("%d", &n);
    int placed = 0;   // 已经被 Elsie 冲洗掉的盘子中的最大标签
    int ans = n;      // 最长可行前缀长度
    for (int i = 0; i < n; i++) {
        int x;
        scanf("%d", &x);
        // 已经冲洗过标签 placed, 之后的盘子标签必须更大; 否则顺序无法保持
        if (x < placed) {
            ans = i;
            break;
        }
        // 标签 j (j <= x) 能放的栈, 一定是这些标签里最小的那个"栈顶标签"对应的栈;
        // 每个 j 只会被赋值一次, 所以下面的循环均摊 O(1)
        for (int j = x; j > 0 && base[j] == 0; j--) base[j] = x;
        // x 只能压在最左边可行的栈上, 且必须先把该栈上比 x 小的盘子冲洗掉,
        // 否则 x 会先于它们被 Elsie 取走, 顺序就被破坏
        while (!items[base[x]].empty() && items[base[x]].back() < x) {
            placed = items[base[x]].back();
            items[base[x]].pop_back();
        }
        items[base[x]].push_back(x);
    }
    printf("%d\n", ans);
    return 0;
}
