#include <cstdio>

using namespace std;

const int MAXN = 3000000;

int prefer[MAXN]; // prefer[i] = 首选 i 号栏位的奶牛数量

int main() {
    int n, k;
    scanf("%d %d", &n, &k);
    for (int t = 0; t < k; t++) {
        int x, y;
        long long a, b;
        scanf("%d %d %lld %lld", &x, &y, &a, &b);
        // x == 0 时这条描述一只奶牛也没带来, 直接跳过, 免得 y 很大时空转
        if (x == 0) continue;
        // f(i) = (A*i + B) mod N, 相邻两项只差 A, 用累加代替每次的乘法
        long long step = a % n;
        long long pos = (a + b) % n; // f(1)
        for (int i = 1; i <= y; i++) {
            prefer[pos] += x;
            pos += step;
            if (pos >= n) pos -= n;
        }
    }

    // 顺序无关, 于是可以先让所有奶牛站在首选栏位, 再从左到右把多出来的奶牛
    // 推到下一个栏位 (这正是她们自己会做的事), 绕一圈即可稳定下来。
    // carry = 扫到当前栏位时被推过来的奶牛数。
    // 第一遍: 走完 N-1 号后剩下的奶牛要回绕到 0 号, 它们的数量留在 carry 里。
    long long carry = 0;
    for (int i = 0; i < n; i++) {
        carry += prefer[i];
        if (carry > 0) carry--; // 留一头占住 i 号, 其余的继续往后推
    }

    // 第二遍: 带着回绕的奶牛再扫一次, 找出第一个怎么推都空的栏位。
    // 扫描时 "carry + prefer[i] == 0" 说明从任何更早的位置推进来的奶牛
    // 都填不满 i 号, 它就是答案。
    for (int i = 0; i < n; i++) {
        carry += prefer[i];
        if (carry > 0) carry--;
        else {
            printf("%d\n", i);
            break;
        }
    }
    return 0;
}
