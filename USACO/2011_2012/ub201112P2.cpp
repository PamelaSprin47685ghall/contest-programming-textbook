#include <algorithm>
#include <cstdio>
#include <vector>

// 记录每头牛在每张照片里的位置 (逆排列): Data[cow] = 该牛在照片中的下标.
struct PhotoOrder {
    std::vector<int> Data;

    void Read(int cowCount) {
        Data.resize(cowCount);
        for (int position = 0; position < cowCount; ++position) {
            int cow;
            scanf("%d", &cow);
            Data[cow - 1] = position;
        }
    }
};

int main() {
    int n;
    scanf("%d", &n);
    PhotoOrder photos[5];
    for (PhotoOrder& photo : photos) photo.Read(n);

    std::vector<int> order(n);
    for (int i = 0; i < n; ++i) order[i] = i;

    // 五张照片里, 若牛 a 排在牛 b 前面至少 3 次, 则 a 应排在 b 前面.
    auto before = [&](int a, int b) {
        int wins = 0;
        for (const PhotoOrder& photo : photos) {
            if (photo.Data[a] < photo.Data[b]) ++wins;
        }
        return 3 <= wins;
    };
    std::sort(order.begin(), order.end(), before);

    for (int cow : order) printf("%d\n", cow + 1);
    return 0;
}