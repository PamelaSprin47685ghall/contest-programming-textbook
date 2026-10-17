#include <cstdio>
#include <vector>

using namespace std;

const int MAXCARD = 100005; // 牌面最大为 2N ≤ 100000

int N;
bool elsieOwns[MAXCARD];
vector<int> bessie, elsie; // 两人手牌，按牌面升序

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; ++i) {
        int card;
        scanf("%d", &card);
        elsieOwns[card] = true;
    }
    // 从小到大扫一遍所有牌面，Elsie 的牌归 Elsie，剩下的都是 Bessie 的，
    // 这样得到的两个序列天然有序，省去排序
    for (int v = 1; v <= 2 * N; ++v) {
        if (elsieOwns[v]) elsie.push_back(v);
        else bessie.push_back(v);
    }

    // 把 Elsie 的牌按从小到大依次当作“下一个要打败的目标”，
    // 依次拿出 Bessie 从小到大的牌去试：能压过当前目标就赢下这一分
    // （用最小的能赢的牌，好牌留到后面）；压不过说明这张牌对任何
    // 尚未击败的目标都没用，直接放弃。
    int points = 0;
    int ei = 0; // 下一个要击败的 Elsie 的牌
    for (int bi = 0; bi < N && ei < N; ++bi) {
        if (bessie[bi] > elsie[ei]) {
            ++points;
            ++ei;
        }
    }
    printf("%d\n", points);
    return 0;
}
