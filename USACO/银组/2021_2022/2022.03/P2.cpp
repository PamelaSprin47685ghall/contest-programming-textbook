#include <cstdio>
#include <cstring>

using namespace std;

const int MAXN = 100005;
const int LETTERS = 18;  // 字母表只用 'a' 到 'r'

char s[MAXN], t[MAXN];
int freqS[LETTERS], freqT[LETTERS];
bool compatible[LETTERS][LETTERS];

// 关键结论: 记 s|X 表示只保留字母集 X 中的字母后得到的串, 则
//   s|X == t|X  <=>  (1) s 与 t 在 X 上各字母频数之和相等
//               且 (2) 对 X 中任意一对字母 {x,y}, s|{x,y} == t|{x,y}.
// 必要性: 限制到子集不会破坏相等; 计数也显然相等.
// 充分性(逆否): 若 s|X != t|X, 要么两者长度不同(计数和不等), 要么存在
//   第一个不同的位置, 那里分别站着字母 x, y; 去掉其它字母后, 前面相同的
//   部分仍相同, 这对 x, y 仍在同一位置, 所以 {x,y} 这一对也不相等.
// 于是只需预处理 18*17/2 = 153 对字母的两两答案.

int main() {
    scanf("%s", s);
    scanf("%s", t);
    int lenS = strlen(s), lenT = strlen(t);
    for (int i = 0; i < lenS; ++i) ++freqS[s[i] - 'a'];
    for (int i = 0; i < lenT; ++i) ++freqT[t[i] - 'a'];

    // 对每一对字母 (x, y), 用双指针同步取出两个串里属于 {x,y} 的字符并比较,
    // 这样不必真的构造缩小后的串, 单对耗时 O(|s| + |t|).
    for (int x = 0; x < LETTERS; ++x) {
        for (int y = x + 1; y < LETTERS; ++y) {
            int i = 0, j = 0;
            bool same = true;
            while (true) {
                while (i < lenS && s[i] - 'a' != x && s[i] - 'a' != y) ++i;
                while (j < lenT && t[j] - 'a' != x && t[j] - 'a' != y) ++j;
                if (i == lenS && j == lenT) break;  // 两边同时取空, 完全相同
                if (i == lenS || j == lenT || s[i] != t[j]) {
                    same = false;  // 一边先取空, 或取出的字符不同
                    break;
                }
                ++i;
                ++j;
            }
            compatible[x][y] = same;
        }
    }

    int Q;
    scanf("%d", &Q);
    char query[LETTERS + 1];
    static char out[MAXN];  // 一次输出全部 Q 个字符, 答案是一整行
    for (int q = 0; q < Q; ++q) {
        scanf("%s", query);
        int len = strlen(query);
        int sumS = 0, sumT = 0;
        for (int k = 0; k < len; ++k) {
            sumS += freqS[query[k] - 'a'];
            sumT += freqT[query[k] - 'a'];
        }
        char answer = 'Y';
        if (sumS != sumT) {
            answer = 'N';  // 条件 (1) 不满足
        } else {
            // 询问串已经按字典序给出, 所以 query[a] < query[b]
            for (int a = 0; a < len && answer == 'Y'; ++a) {
                for (int b = a + 1; b < len; ++b) {
                    if (!compatible[query[a] - 'a'][query[b] - 'a']) {
                        answer = 'N';  // 条件 (2) 不满足
                        break;
                    }
                }
            }
        }
        out[q] = answer;
    }
    out[Q] = '\0';
    printf("%s\n", out);
    return 0;
}
