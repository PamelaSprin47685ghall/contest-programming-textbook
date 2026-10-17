#include <cstdio>
#include <cstring>

using namespace std;

const int LETTERS = 52;    // 26 个小写字母 + 26 个大写字母
const int MAXLEN = 100005; // 题目保证所有字符串长度之和不超过 1e5

char s[MAXLEN], t[MAXLEN];
int to[LETTERS];     // to[c]：字符 c 必须变成的字符；-1 表示 c 没在输入串中出现
int inDeg[LETTERS];  // 只统计非自环边的入度
int remain[LETTERS]; // 剥叶子过程中剩下的入度，>0 说明该点还在环上
bool inOut[LETTERS]; // 该字符是否出现在输出串中
int leafQ[LETTERS];

int id(char c) {
    if ('a' <= c && c <= 'z') return c - 'a';
    return 26 + (c - 'A');
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%s", s);
        scanf("%s", t);
        for (int c = 0; c < LETTERS; c++) {
            to[c] = -1;
            inOut[c] = false;
        }
        int len = strlen(s);
        bool possible = true;
        for (int i = 0; i < len; i++) {
            int a = id(s[i]), b = id(t[i]);
            inOut[b] = true;
            // 同一个字符在不同位置要变成不同字符：一次整体替换无法区分，无解
            if (to[a] != -1 && to[a] != b) possible = false;
            to[a] = b;
        }
        int distinctOut = 0;
        for (int c = 0; c < LETTERS; c++) {
            if (inOut[c]) distinctOut++;
        }
        // s != t 时任何一次替换都会让某个字符从串中彻底消失，
        // 因此输出串用满 52 个字符时不可能完成
        if (distinctOut == LETTERS && strcmp(s, t) != 0) possible = false;
        if (!possible) {
            printf("-1\n");
            continue;
        }

        int answer = 0;
        for (int c = 0; c < LETTERS; c++) {
            inDeg[c] = 0;
            remain[c] = 0;
        }
        for (int c = 0; c < LETTERS; c++) {
            if (to[c] != -1 && to[c] != c) {
                answer++; // 每条非自环的边至少要花一次替换把它消掉
                inDeg[to[c]]++;
            }
        }

        // 把图看成以「非自环边」构成的有向图：每个点出度不超过 1。
        // 剥掉所有入度为 0 的点，剩下的点恰好都落在环上
        for (int c = 0; c < LETTERS; c++) remain[c] = inDeg[c];
        int head = 0, tail = 0;
        for (int c = 0; c < LETTERS; c++) {
            if (to[c] != -1 && to[c] != c && remain[c] == 0) leafQ[tail++] = c;
        }
        while (head < tail) {
            int a = leafQ[head++];
            int b = to[a];
            remain[b]--;
            if (remain[b] == 0 && to[b] != -1 && to[b] != b) leafQ[tail++] = b;
        }

        // 环上的点若入度全为 1，说明整个连通块就是一个纯环（没有别的点指向它），
        // 这种环无法借用块外的临时字符，必须多花一次替换来拆开
        for (int c = 0; c < LETTERS; c++) {
            if (remain[c] == 0 || to[c] == -1 || to[c] == c) continue;
            int a = c;
            bool pureCycle = true;
            do {
                remain[a] = 0; // 置 0 表示这个环已经统计过
                if (inDeg[a] != 1) pureCycle = false;
                a = to[a];
            } while (a != c);
            if (pureCycle) answer++;
        }
        printf("%d\n", answer);
    }
    return 0;
}
