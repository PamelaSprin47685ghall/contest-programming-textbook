#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 200005;
// 最终串可能长到 2^100000 级别，但只关心前 1e18 个字符，
// 所以子树长度一律封顶在 1e18，既避免溢出也不影响答案。
const long long LIMIT = 1000000000000000000LL;

// 叶结点保存单个字符，内部结点表示它两棵子树内容的拼接。
// 同一棵子树在最终串里可能出现很多次，这里只保存一份、用下标共享。
struct Node {
    long long size;   // 展开后的长度，封顶 LIMIT
    int left, right;  // 内部结点的左右子树下标，叶结点为 0
    char value;       // 叶结点的字符，内部结点为 '\0'
};
// sum|s| <= 2e5，每次操作最多新建 |s|-1 个内部结点，再加上 26 个叶结点
Node node[MAXN + 30];
int nodeCnt = 1;      // 0 号位置空出来，表示"不存在"

char opFrom[MAXN];    // 第 i 次操作被替换的字符
char opText[MAXN];    // 所有操作的替换串依次拼接存放
int opStart[MAXN];    // 第 i 个替换串在 opText 中的起点
int opLen[MAXN];      // 第 i 个替换串的长度

int newNode(char value, long long size, int left, int right) {
    node[nodeCnt].value = value;
    node[nodeCnt].size = size;
    node[nodeCnt].left = left;
    node[nodeCnt].right = right;
    return nodeCnt++;
}

// 遍历时的一个待处理区间：[from, to] 是结点 id 内部（从 1 开始）的一段位置
struct Task {
    int id;
    long long from, to;
};

int main() {
    long long l, r;
    int n;
    scanf("%lld%lld%d", &l, &r, &n);

    int total = 0;
    for (int i = 0; i < n; ++i) {
        char cbuf[4];
        scanf("%s", cbuf);
        opFrom[i] = cbuf[0];
        opStart[i] = total;
        scanf("%s", opText + total);
        int len = 0;
        while (opText[total + len] != '\0') {
            ++len;
        }
        opLen[i] = len;
        total += len;
    }

    // cur[c]：以字符 c 为起点，依次经过"已经处理过的那些操作"后得到的串
    int cur[26];
    for (int c = 0; c < 26; ++c) {
        cur[c] = newNode(char('a' + c), 1, 0, 0);
    }

    // 关键：倒序处理操作。处理第 i 次操作时，所有更晚发生的操作
    // 都已经作用在 cur[] 上，于是这次替换只需要把若干现成的子树拼接起来。
    for (int i = n - 1; i >= 0; --i) {
        int result = 0;
        for (int j = 0; j < opLen[i]; ++j) {
            int toMerge = cur[opText[opStart[i] + j] - 'a'];
            if (result == 0) {
                // 替换串只有 1 个字符时直接沿用同一棵子树（不新建结点）。
                // 否则同一个字符被反复替换时，结点数会随步数线性增长成
                // 指数级规模，复杂度退化。
                result = toMerge;
            } else {
                long long sz = node[result].size + node[toMerge].size;
                if (sz > LIMIT) {
                    sz = LIMIT;
                }
                result = newNode('\0', sz, result, toMerge);
            }
        }
        cur[opFrom[i] - 'a'] = result;
    }

    // 从根出发只展开与 [l, r] 相交的部分：完全在区间内的子树直接输出，
    // 完全在区间外的子树整棵剪掉。用显式栈代替递归，避免深度过大爆栈。
    vector<Task> stk;
    Task first;
    first.id = cur[0];
    first.from = l;
    first.to = r;
    stk.push_back(first);

    static char out[MAXN];
    int outCnt = 0;
    while (!stk.empty()) {
        Task t = stk.back();
        stk.pop_back();
        long long from = t.from, to = t.to;
        if (from < 1) {
            from = 1;
        }
        if (to > node[t.id].size) {
            to = node[t.id].size;
        }
        if (from > to) {
            continue;
        }
        if (node[t.id].value != '\0') {
            out[outCnt++] = node[t.id].value;
            continue;
        }
        long long leftSize = node[node[t.id].left].size;
        // 先压右子树、再压左子树，出栈顺序才是从左到右
        if (to > leftSize) {
            Task rightTask;
            rightTask.id = node[t.id].right;
            rightTask.from = from - leftSize;  // 可能为负，上面会夹紧
            rightTask.to = to - leftSize;
            stk.push_back(rightTask);
        }
        if (from <= leftSize) {
            Task leftTask;
            leftTask.id = node[t.id].left;
            leftTask.from = from;
            leftTask.to = to;
            stk.push_back(leftTask);
        }
    }
    out[outCnt] = '\0';
    printf("%s\n", out);
    return 0;
}
