#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100000;

int n;
int fj[MAXN + 5];               // FJ 每局的手势, 0=H, 1=P, 2=S
int winPre[MAXN + 5][3];        // winPre[i][g]: 前 i 局一直出 g 的胜场数
int winSuf[MAXN + 5][3];        // winSuf[i][g]: 第 i 局起一直出 g 的胜场数
int bestPre[MAXN + 5];          // 前 i 局固定一种手势能取得的最多胜场
int bestSuf[MAXN + 5];          // 第 i 局起固定一种手势能取得的最多胜场
char buf[16];

// 手势编号 0=H, 1=P, 2=S; 蹄子赢剪刀, 剪刀赢布, 布赢蹄子
int beats(int g) {
    return (g + 1) % 3;
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%s", buf);
        if (buf[0] == 'H') fj[i] = 0;
        else if (buf[0] == 'P') fj[i] = 1;
        else fj[i] = 2;
    }

    // 前缀: 只关心 "一直出 g 能否赢下第 i 局"
    for (int i = 1; i <= n; i++) {
        int r = beats(fj[i]);
        for (int g = 0; g < 3; g++) {
            winPre[i][g] = winPre[i - 1][g] + (g == r);
        }
    }
    // 后缀: 递推方向相反, 从 n 向 1 累加
    for (int i = n; i >= 1; i--) {
        int r = beats(fj[i]);
        for (int g = 0; g < 3; g++) {
            winSuf[i][g] = winSuf[i + 1][g] + (g == r);
        }
    }

    for (int i = 0; i <= n; i++) {
        for (int g = 0; g < 3; g++) {
            bestPre[i] = max(bestPre[i], winPre[i][g]);
            bestSuf[i] = max(bestSuf[i], winSuf[i][g]);
        }
    }

    // 枚举换手势的位置 x: 前 x 局一种手势, 后 n-x 局另一种手势
    int answer = 0;
    for (int x = 0; x <= n; x++) {
        answer = max(answer, bestPre[x] + bestSuf[x + 1]);
    }
    printf("%d\n", answer);
    return 0;
}
