#include <cstdio>
#include <vector>

// 求从节点 no 出发沿 b 走回 no 的循环节大小.
int FindCycleSize(const std::vector<int>& next, std::vector<int>& visited,
                  int start) {
    int count = 0;
    int current = start;
    visited[current] = true;
    while (true) {
        ++count;
        current = next[current];
        visited[current] = true;
        if (current == start) return count;
    }
}

int main() {
    int n;
    scanf("%d", &n);
    std::vector<int> a(n), b(n), position(n), visited(n, false);
    for (int& value : a) scanf("%d", &value);
    for (int& value : b) scanf("%d", &value);
    // position[value] = 该值在 b 中的位置; b[i]-1 是 0 基下标.
    for (int i = 0; i < n; ++i) position[b[i] - 1] = i;
    // 构造置换 next: a[i] 最终要移到 b 中该值所在的位置.
    for (int i = 0; i < n; ++i) b[i] = position[a[i] - 1];

    int cycles = 0, largest = -1;
    for (int i = 0; i < n; ++i) {
        if (visited[i]) continue;
        int size = FindCycleSize(b, visited, i);
        if (size == 1) continue; // 单元素循环不是真正的移动
        if (largest < size) largest = size;
        ++cycles;
    }
    printf("%d %d\n", cycles, largest);
    return 0;
}