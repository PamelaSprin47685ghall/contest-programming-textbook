#include <cstdio>
#include <vector>

// 第 i 级 S(i) = S(i-1) + 中间段 + S(i-1), 中间段是 "m" 后跟 i+2 个 "o".
// lengths[i] 记录 S(i) 的长度.
std::vector<int> BuildLengths(int n) {
    std::vector<int> lengths(1, 0);
    while (lengths.back() < n) {
        int level = (int)lengths.size() - 1;
        // S(level+1) = S(level) + (level+3) + S(level).
        lengths.push_back(lengths.back() * 2 + (level + 3));
    }
    return lengths;
}

// 在 S(level) 里找第 n 个字符 (1 起始).
// lengths[level] 恰好是左半边 S(level-1) 的长度.
char FindChar(int n, int level, const std::vector<int>& lengths) {
    int leftSize = lengths[level];
    int middleSize = level + 3;
    if (n <= leftSize) return FindChar(n, level - 1, lengths);  // 在左半边
    if (n <= leftSize + middleSize) {
        // 在中间段: 第 1 个是 'm', 其余是 'o'.
        return (n == leftSize + 1) ? 'm' : 'o';
    }
    // 在右半边, 平移回左半边的坐标.
    return FindChar(n - (leftSize + middleSize), level - 1, lengths);
}

int main() {
    int n;
    scanf("%d", &n);
    std::vector<int> lengths = BuildLengths(n);
    printf("%c\n", FindChar(n, (int)lengths.size() - 1, lengths));
    return 0;
}