#include <cstdio>

using namespace std;

const int MAXN = 100005;
const int MAXM = 10005;

// 模拟思路(贪心): 文件夹窗口只会往下滚, 所以文件夹窗口的上沿 firstFolder
// 在新窗口上沿的"所有邮件都归档完"时就可以立刻下移一格 —— 移走的那档已经空了,
// 而新露出的一档只会让更多邮件可归档, 所以能移就移, 不会吃亏.
//
// 邮件窗口用 [firstEmail, lastEmail] 这段下标表示: 它是"当前显示的 k 封邮件",
// 区间里可能有已经归档的邮件(归档后下面的邮件会顶上来, 区间右端补一封新的),
// 所以区间内未归档的邮件恰好是 k 封. firstEmail 之前的邮件就是被滚过去(skipped)
// 的那些; 邮件列表滚到底之后, 每次归档都会让窗口上移一格, 把 lastEmail 之上
// 最近的一封 skipped 邮件重新带进窗口.
//
// 堆里存窗口内所有未归档邮件, 键是它要归档到的文件夹编号. 堆顶就是窗口里
// "最着急"(文件夹最靠上)的那封: 只要它的文件夹还在窗口范围内, 就一定能归档,
// 而且应该立刻归档.
int folder[MAXN];     // 每封邮件的目标文件夹(0 起编号)
bool filed[MAXN];     // 是否已经归档
int remaining[MAXM];  // 每个文件夹还有几封未归档的邮件

int heap[MAXN];  // 小根堆, 存邮件下标, 比较键是 folder
int heapSize;

bool HeapLess(int a, int b) {
    if (folder[a] != folder[b]) return folder[a] < folder[b];
    return a < b;
}

void HeapPush(int v) {
    heap[heapSize] = v;
    int cur = heapSize++;
    while (cur > 0) {
        int up = (cur - 1) / 2;
        if (!HeapLess(heap[cur], heap[up])) break;
        int tmp = heap[cur];
        heap[cur] = heap[up];
        heap[up] = tmp;
        cur = up;
    }
}

int HeapPop() {
    int top = heap[0];
    heap[0] = heap[--heapSize];
    int cur = 0;
    while (true) {
        int child = 2 * cur + 1;
        if (child >= heapSize) break;
        if (child + 1 < heapSize && HeapLess(heap[child + 1], heap[child])) ++child;
        if (!HeapLess(heap[child], heap[cur])) break;
        int tmp = heap[cur];
        heap[cur] = heap[child];
        heap[child] = tmp;
        cur = child;
    }
    return top;
}

void Solve() {
    int m, n, k;
    scanf("%d%d%d", &m, &n, &k);
    for (int f = 0; f < m; ++f) remaining[f] = 0;
    heapSize = 0;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &folder[i]);
        --folder[i];
        ++remaining[folder[i]];
        filed[i] = false;
    }

    int firstFolder = 0;
    int firstEmail = 0;
    int lastEmail = k - 1;
    for (int i = 0; i < k; ++i) HeapPush(i);

    // 主体: 只要邮件列表还没到底, 就尽量往右推进
    while (lastEmail < n - 1 && firstFolder < m) {
        if (remaining[firstFolder] == 0) {
            ++firstFolder;  // 这一档已经清空, 文件夹窗口下移
        } else if (heapSize > 0 && folder[heap[0]] < firstFolder + k) {
            int e = HeapPop();
            if (e >= firstEmail) {  // 还在窗口里(没被滚过去)
                filed[e] = true;
                --remaining[folder[e]];
                ++lastEmail;  // 归档掉一封, 右端补进一封
                HeapPush(lastEmail);
            }
        } else {
            // 窗口里没有一封能归档: 只能往下滚一格. 被挤出去的邮件之后
            // 要靠"在列表底部归档"一格一格带回来, 所以暂时不进堆.
            while (firstEmail < n && filed[firstEmail]) ++firstEmail;
            ++firstEmail;
            ++lastEmail;
            HeapPush(lastEmail);
        }
    }

    bool ok = true;
    while (firstFolder < m) {
        if (remaining[firstFolder] == 0) {
            ++firstFolder;
            continue;
        }
        if (heapSize == 0) {  // 窗口里已经没有邮件, 但还有邮件没归位
            ok = false;
            break;
        }
        int e = HeapPop();
        if (e < firstEmail || filed[e]) continue;  // 已经被滚出窗口的旧记录
        if (folder[e] >= firstFolder + k) {
            // 堆顶是窗口里文件夹最靠上的邮件; 它已经超出当前文件夹窗口的范围,
            // 而 firstFolder 又因为本档还有邮件不能下移, 再也归档不了, 无解.
            ok = false;
            break;
        }
        filed[e] = true;
        --remaining[folder[e]];
        // 邮件列表到底了: 归档会让窗口上移一格, 把上面最近的一封邮件带回来
        while (firstEmail > 0) {
            --firstEmail;
            if (!filed[firstEmail]) {
                HeapPush(firstEmail);
                break;
            }
        }
    }
    printf(ok ? "YES\n" : "NO\n");
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) Solve();
    return 0;
}
