#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

struct Statement {
    char dir;
    int p;
};

int main() {
    int n;
    scanf("%d", &n);
    vector<Statement> stmts(n);
    vector<int> candidatePoints;

    for (int i = 0; i < n; ++i) {
        scanf(" %c%d", &stmts[i].dir, &stmts[i].p);
        candidatePoints.push_back(stmts[i].p);
    }

    // 最优的隐藏坐标必然可以取在某头牛陈述的关键边界点 p[i] 处
    int minLiars = n;

    for (int pos : candidatePoints) {
        int liars = 0;
        for (const Statement& s : stmts) {
            if (s.dir == 'G' && pos < s.p) {
                // 声称 >= p, 但实际 < p, 为说谎
                ++liars;
            } else if (s.dir == 'L' && pos > s.p) {
                // 声称 <= p, 但实际 > p, 为说谎
                ++liars;
            }
        }
        if (liars < minLiars) {
            minLiars = liars;
        }
    }

    printf("%d\n", minLiars);
    return 0;
}
