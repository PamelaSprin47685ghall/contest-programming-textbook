#include <algorithm>
#include <cstdio>
#include <vector>

int main() {
    int n;
    scanf("%d", &n);
    std::vector<int> bales(n);
    for (int& bale : bales) scanf("%d", &bale);

    int sum = 0;
    for (int bale : bales) sum += bale;
    int average = sum / n;

    // 高于平均数的部分就是需要移走的干草捆数.
    int answer = 0;
    for (int bale : bales) answer += std::max(0, bale - average);
    printf("%d\n", answer);
    return 0;
}