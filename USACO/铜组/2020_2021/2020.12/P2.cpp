#include <cstdio>
#include <vector>

using namespace std;

int main() {
    int n;
    scanf("%d", &n);
    vector<int> p(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &p[i]);
    }

    int validPhotos = 0;

    for (int i = 0; i < n; ++i) {
        int sum = 0;
        for (int j = i; j < n; ++j) {
            sum += p[j];
            int count = j - i + 1;

            if (sum % count == 0) {
                int avg = sum / count;
                bool hasAvg = false;
                for (int k = i; k <= j; ++k) {
                    if (p[k] == avg) {
                        hasAvg = true;
                        break;
                    }
                }
                if (hasAvg) {
                    ++validPhotos;
                }
            }
        }
    }

    printf("%d\n", validPhotos);
    return 0;
}
