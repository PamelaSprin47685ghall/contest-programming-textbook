#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 100000;

int to[MAXN + 1];    // 位置 i 上的牛洗牌后前往的位置 a_i
int indeg[MAXN + 1]; // 仍可能长期有牛的位置中，有多少个位置洗牌后指向 i

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &to[i]);
        indeg[to[i]]++;
    }

    // 入度为 0 的位置收不到任何牛，一次洗牌后就永远空着；它空掉之后，
    // 它对后继位置的"贡献"消失，可能让后继的入度也降为 0，于是可以连锁
    // 地删下去。最终没被删掉的位置恰好是功能图上的环，环上的位置始终有牛。
    vector<int> pending;
    for (int i = 1; i <= n; i++)
        if (indeg[i] == 0) pending.push_back(i);

    int answer = n;
    while (!pending.empty()) {
        int cur = pending.back();
        pending.pop_back();
        answer--; // cur 被确认最终会空掉
        int nxt = to[cur];
        if (--indeg[nxt] == 0) pending.push_back(nxt);
    }

    printf("%d\n", answer);
    return 0;
}
