#include <cstdio>
#include <vector>

using namespace std;

// 十进制数 x 的数位之和.
int DigitSum(int x) {
    int sum = 0;
    while (x != 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

// 两个数相加是否产生进位: 不进位时数位之和恰好等于和的数位之和.
bool HasCarry(int a, int b) {
    return DigitSum(a) + DigitSum(b) != DigitSum(a + b);
}

// 枚举每个干草捆是否加入集合, 记录不产生进位时能选出的最大个数.
struct Search {
    const vector<int>* weights;
    int answer = 0;

    void Compute(int index, int size, int sum) {
        answer = (size > answer) ? size : answer;
        if (index == (int)weights->size()) return;

        // 不选当前干草捆.
        Compute(index + 1, size, sum);
        // 若加上当前干草捆不产生进位才选它.
        if (!HasCarry(sum, (*weights)[index])) {
            Compute(index + 1, size + 1, sum + (*weights)[index]);
        }
    }
};

int main() {
    int n;
    scanf("%d", &n);
    vector<int> weights(n);
    for (int& weight : weights) scanf("%d", &weight);

    Search search;
    search.weights = &weights;
    search.Compute(0, 0, 0);
    printf("%d\n", search.answer);
    return 0;
}