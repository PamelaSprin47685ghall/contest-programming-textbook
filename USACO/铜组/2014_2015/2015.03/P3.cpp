#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

struct Bale {
    int size;
    int position;
};

bool operator<(const Bale& b0, const Bale& b1) { return b0.position < b1.position; }

// 贝茜被困在干草堆 aL 与 aR 之间，能否逃出（向左右扩张直到越界）。
bool canEscape(const vector<Bale>& bales, int aL, int aR) {
    if (aL < 0 || (int)bales.size() <= aR) return true;
    int distance = bales[aR].position - bales[aL].position;
    // 左堆不够大就向左扩张，右堆不够大就向右扩张；都够大则被困死。
    if (bales[aL].size < distance) return canEscape(bales, aL - 1, aR);
    if (bales[aR].size < distance) return canEscape(bales, aL, aR + 1);
    return false;
}

int main() {
    int n;
    scanf("%d", &n);
    vector<Bale> bales(n);
    for (int i = 0; i < n; ++i) scanf("%d%d", &bales[i].size, &bales[i].position);
    sort(bales.begin(), bales.end());
    int answer = 0;
    // 枚举贝茜可能被困住的每一对相邻干草堆，累加无法逃出的间距。
    for (int i = 1; i < (int)bales.size(); ++i) {
        if (!canEscape(bales, i - 1, i)) answer += bales[i].position - bales[i - 1].position;
    }
    printf("%d\n", answer);
    return 0;
}
