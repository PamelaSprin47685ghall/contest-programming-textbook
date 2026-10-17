#include <algorithm>
#include <cstdio>

using namespace std;

int main() {
    int nums[7];
    for (int i = 0; i < 7; ++i) {
        scanf("%d", &nums[i]);
    }
    sort(nums, nums + 7);

    int a = nums[0];
    int b = nums[1];
    int c = nums[6] - a - b;

    printf("%d %d %d\n", a, b, c);
    return 0;
}
