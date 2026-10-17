#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 100005;

char leftSay[MAXN];   // 每头奶牛对左邻居的声明
char rightSay[MAXN];  // 每头奶牛对右邻居的声明

int main() {
    int T, C;
    scanf("%d %d", &T, &C);
    while (T--) {
        int N;
        scanf("%d", &N);
        scanf("%s", leftSay);
        scanf("%s", rightSay);

        // 按 (对左声明, 对右声明) 把奶牛分成 JJ、JN、NJ、NN 四类。
        // 关键观察：相邻两头奶牛交换的声明总是相同——同类农场主都说对方是 J，不同类都说对方是 N，
        // 所以两头奶牛中间的那个声明会同时出现在左串（右边那头的"左"）和右串（左边那头的"右"）里。
        vector<int> jj, jn, nj, nn;
        int leftN = 0;
        for (int i = 0; i < N; ++i) {
            if (leftSay[i] == 'N') ++leftN;
            if (leftSay[i] == 'J') {
                if (rightSay[i] == 'J') jj.push_back(i);
                else jn.push_back(i);
            } else {
                if (rightSay[i] == 'J') nj.push_back(i);
                else nn.push_back(i);
            }
        }

        // 三个必要条件：
        // 1. JN 类与 NJ 类数量相同（等价于两条串里 J 的个数相同）；
        // 2. 左串里 N 的个数是偶数——每遇到一个 N 的声明，相邻两牛归属就互换一次，绕圆一周必须换回原归属；
        // 3. JJ 类与 NN 类不同时存在，除非有 JN / NJ 类奶牛把两段接起来。
        bool possible = (int)jn.size() == (int)nj.size() && leftN % 2 == 0
                        && (jj.empty() || nn.empty() || !jn.empty() || !nj.empty());
        if (possible) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
        if (!C || !possible) continue;

        vector<int> order;
        if ((int)jj.size() == N || (int)nn.size() == N) {
            // 全是 JJ：全部归约翰，任意顺序；全是 NN：归属正好逐个交替，也任意顺序。
            for (int i = 0; i < N; ++i) order.push_back(i);
        } else {
            // 依次放：[JJ…] [一个 JN] [NN…] [（NJ, JN）若干组] [最后一个 NJ]。
            // JN 的右侧声明、NN 的两侧声明、NJ 与 JN 之间的声明都按 N 相接；接缝处用 J 相接。
            for (int i = 0; i < (int)jj.size(); ++i) order.push_back(jj[i]);
            order.push_back(jn.back());
            jn.pop_back();
            for (int i = 0; i < (int)nn.size(); ++i) order.push_back(nn[i]);
            while (!jn.empty() && !nj.empty()) {
                order.push_back(nj.back());
                nj.pop_back();
                order.push_back(jn.back());
                jn.pop_back();
            }
            order.push_back(nj.back());
        }

        for (int i = 0; i < N; ++i) {
            printf("%d", order[i] + 1);
            printf(i + 1 == N ? "\n" : " ");
        }

        // 归属串：相邻两头奶牛之间的声明是左串在右边那头奶牛处的字符（等于左边那头的右声明），
        // 为 N 表示两头归属不同，所以在那里翻转归属。
        char who[MAXN];
        int owner = 0;  // 0 表示 J，1 表示 N
        who[0] = 'J';
        for (int i = 1; i < N; ++i) {
            if (leftSay[order[i]] == 'N') owner = 1 - owner;
            who[i] = owner ? 'N' : 'J';
        }
        who[N] = 0;
        printf("%s\n", who);
    }
    return 0;
}
