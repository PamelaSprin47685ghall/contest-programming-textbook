#include <cstdio>

using namespace std;

// 队首的牛被挪到中间后，再回到队首至少要乘上 3/2，所以一头牛在 t 内最多当 O(log t) 次队首。
// 问牛 c 在时刻 t 的位置：它在 2c 之前还没进队伍；进队后停在队首只持续 1 个时刻，
// 然后被放到当时时刻的一半处，再逐格往前走。
long long cowPosition(long long cow, long long time) {
    if (time < 2 * cow) return cow;
    long long walked = time - (2 * cow - 1);
    long long position = cow;
    long long now = 2 * cow - 1;
    while (true) {
        if (walked <= position) return position - walked;
        walked -= position;
        now += position + 1;
        walked -= 1;
        position = now / 2;
    }
}

// 问时刻 t 的位置 x 是哪头牛。位置超过 t/2 的还没被挤过，编号就是位置本身；
// 恰好落在 t/2 的是上一时刻的队首。更靠前时可以一次跳过 floor((t-2x)/3) 步，
// 每跳一次都会碰到一次"队首被挪走"，所以也只跳 O(log t) 次。
long long cowAt(long long position, long long time) {
    while (time > 0) {
        long long middle = time / 2;
        if (position > middle) return position;
        if (position == middle) {
            position = 0;
            time -= 1;
            continue;
        }
        long long step = (time - 2 * position) / 3;
        if (step < 1) step = 1;
        position += step;
        time -= step;
    }
    return position;
}

int main() {
    int queryCount;
    scanf("%d", &queryCount);
    for (int i = 0; i < queryCount; ++i) {
        int kind;
        long long first, time;
        scanf("%d %lld %lld", &kind, &first, &time);
        if (kind == 1) printf("%lld\n", cowPosition(first, time));
        else printf("%lld\n", cowAt(first, time));
    }
    return 0;
}
