#include <algorithm>
#include <iostream>

// 计算区间 [a, b] 与 [c, d] 的交集长度
int Intersect(int a, int b, int c, int d) {
    int left = std::max(a, c);
    int right = std::min(b, d);
    return std::max(0, right - left + 1);
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
    if (std::cin >> t) {
        while (t--) {
            int n;
            std::cin >> n;
            std::cout << Solve(n) << "\n";
        }
    }
    return 0;
}
