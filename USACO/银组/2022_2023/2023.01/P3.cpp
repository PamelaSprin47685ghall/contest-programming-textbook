#include <cstdio>

using namespace std;

const int MAXN = 100005;   // N 的上限
const int MAXR = 1000006;  // 总步数 sum A_i 不超过 1e6

int a[MAXN + 1];    // a[0..N-1] 为输入，额外令 a[N] = 0
int leftPos[MAXR];  // 每个「L 转 R」发生的位置
int rightPos[MAXR]; // 每个「R 转 L」发生的位置
char route[MAXR];

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    a[n] = 0;

    // 最优转向次数为 sum |A_i - A_{i+1}| / 2 - 1（令 A_{-1} = A_N = 0）：
    // 差值处必然要转向，而按下面的括号配对构造恰好能达到这个下界。
    // A_i 增大说明在位置 i+1 处有一段「先 L 后 R」，记作左括号；
    // A_i 减小说明在位置 i+1 处有一段「先 R 后 L」，记作右括号。
    int nl = 0, nr = 0;
    for (int t = 0; t < a[0] / 2; t++) leftPos[nl++] = 0;
    for (int i = 0; i + 1 < n; i++) {
        if (a[i] > a[i + 1]) {
            for (int t = 0; t < (a[i] - a[i + 1]) / 2; t++) rightPos[nr++] = i + 1;
        } else {
            for (int t = 0; t < (a[i + 1] - a[i]) / 2; t++) leftPos[nl++] = i + 1;
        }
    }
    for (int t = 0; t < a[n - 1] / 2; t++) rightPos[nr++] = n;

    // 从最靠右的一对括号开始：先向右走到右括号，再向左走到左括号，
    // 依次往复。括号序列「任意前缀左括号都多于右括号」保证了这种走法合法
    int x = 0, len = 0;
    for (int k = nl - 1; k >= 0; k--) {
        while (x < rightPos[k]) {
            x++;
            route[len++] = 'R';
        }
        while (x > leftPos[k]) {
            x--;
            route[len++] = 'L';
        }
    }
    route[len] = '\0';
    printf("%s\n", route);
    return 0;
}
