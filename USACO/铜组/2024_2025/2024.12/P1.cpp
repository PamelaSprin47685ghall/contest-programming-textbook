#include <algorithm>
#include <cstdio>

using namespace std;

// 计算区间 [a, b] 与 [c, d] 的交集长度
int Intersect(int a, int b, int c, int d) {
    int left = max(a, c);
    int right = min(b, d);
    return max(0, right - left + 1);
}

int Solve(int n) {
    int start = 5, end = 49;
    int powerOfTen = 1;
    int answer = 0;
    while (powerOfTen < 100000000) {
        powerOfTen *= 10;
        start += 4 * powerOfTen;
        end = 5 * powerOfTen - 1;
        answer += Intersect(start, end, 2, n);
    }
    return answer;
}

int main() {
    int t;
    if (scanf("%d", &t) == 1) {
        while (t--) {
            int n;
            scanf("%d", &n);
            printf("%d\n", Solve(n));
        }
    }
    return 0;
}
