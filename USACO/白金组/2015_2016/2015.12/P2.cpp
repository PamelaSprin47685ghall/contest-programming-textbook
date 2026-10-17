#include <cstdio>
#include <set>
#include <vector>

using namespace std;

const int MAXCARD = 100000; // 牌面最大值，即 2N 的上限

bool heldByElsie[MAXCARD + 1]; // 标记每张牌在不在 Elsie 手里，其余就是 Bessie 的牌

// Bessie 手里的全部牌，按牌面从小到大放进集合，
// 方便随时找"刚好吃得掉对方的那张牌"并把它取走
set<int> buildHand(int n) {
    set<int> hand;
    for (int card = 1; card <= 2 * n; ++card) {
        if (!heldByElsie[card]) hand.insert(card);
    }
    return hand;
}

int main() {
    int n;
    scanf("%d", &n);
    vector<int> elsie(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &elsie[i]);
        heldByElsie[elsie[i]] = true;
    }

    // 若前 i 局都用"大牌赢"：每局用刚好压过对方的最小牌去赢；
    // 压不过时这一局必输，就丢掉自己最小的牌，把大牌留给后面的对局。
    vector<int> highWins(n + 1, 0); // highWins[i]：前 i 局按"大牌赢"最多赢几局
    set<int> hand = buildHand(n);
    for (int i = 0; i < n; ++i) {
        highWins[i + 1] = highWins[i];
        set<int>::iterator bigger = hand.upper_bound(elsie[i]);
        if (bigger != hand.end()) {
            hand.erase(bigger);
            highWins[i + 1] += 1;
        } else {
            hand.erase(hand.begin());
        }
    }

    // 若从第 i 局起改成"小牌赢"：从最后一局往前贪心，
    // 用刚好小于对方的最大牌去赢；赢不了就丢掉自己最大的牌，把小牌留给前面。
    vector<int> lowWins(n + 1, 0); // lowWins[i]：第 i 到第 N-1 局按"小牌赢"最多赢几局
    hand = buildHand(n);
    for (int i = n - 1; i >= 0; --i) {
        lowWins[i] = lowWins[i + 1];
        set<int>::iterator smaller = hand.lower_bound(elsie[i]);
        if (smaller != hand.begin()) {
            --smaller; // 退一格就是小于对方的最大牌
            hand.erase(smaller);
            lowWins[i] += 1;
        } else {
            hand.erase(--hand.end());
        }
    }

    // 在两个数组里各自最优的两段可能重复用到同一张牌，
    // 但把重复的那张换成没用到的那张，胜局数不会变少，所以拼接方案总是可行的。
    int answer = 0;
    for (int i = 0; i <= n; ++i) {
        if (highWins[i] + lowWins[i] > answer) answer = highWins[i] + lowWins[i];
    }
    printf("%d\n", answer);
    return 0;
}
