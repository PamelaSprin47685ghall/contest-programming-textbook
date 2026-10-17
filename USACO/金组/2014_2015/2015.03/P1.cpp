#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

// ---------- 高精度十进制数 ----------
// N 可以大到 10^100, 员工编号和答案都远超 64 位整数, 所以编号用字符串存,
// 子树大小用 10^9 进制的高精度数存 (只需要加一、减一、除以 2 和判奇偶)。
const unsigned int BIG_BASE = 1000000000u;

typedef vector<unsigned int> Big; // 低位在前, 空向量表示 0

bool isZero(const Big &a) { return a.empty(); }

bool isOdd(const Big &a) { return !a.empty() && (a[0] & 1u); }

Big bigAdd(const Big &a, const Big &b) {
    Big c;
    unsigned int carry = 0;
    for (size_t i = 0; i < a.size() || i < b.size() || carry; i++) {
        unsigned long long cur = carry;
        if (i < a.size()) cur += a[i];
        if (i < b.size()) cur += b[i];
        c.push_back((unsigned int)(cur % BIG_BASE));
        carry = (unsigned int)(cur / BIG_BASE);
    }
    return c;
}

Big bigInc(const Big &a) {
    Big c = a;
    for (size_t i = 0; i < c.size(); i++) {
        c[i]++;
        if (c[i] < BIG_BASE) return c; // 没有继续进位
        c[i] = 0;
    }
    c.push_back(1);
    return c;
}

Big bigDec(const Big &a) { // 调用前保证 a >= 1
    Big c = a;
    for (size_t i = 0; i < c.size(); i++) {
        if (c[i] > 0) {
            c[i]--;
            break;
        }
        c[i] = BIG_BASE - 1;
    }
    while (!c.empty() && c.back() == 0) c.pop_back();
    return c;
}

Big bigHalve(const Big &a) { // 向下取整除以 2
    Big c(a.size(), 0);
    unsigned long long rem = 0;
    for (int i = (int)a.size() - 1; i >= 0; i--) {
        unsigned long long cur = rem * BIG_BASE + a[i];
        c[i] = (unsigned int)(cur / 2);
        rem = cur & 1ULL;
    }
    while (!c.empty() && c.back() == 0) c.pop_back();
    return c;
}

string bigStr(const Big &a) {
    if (a.empty()) return "0";
    char buf[24];
    string s;
    snprintf(buf, sizeof(buf), "%u", a.back());
    s = buf;
    for (int i = (int)a.size() - 2; i >= 0; i--) {
        snprintf(buf, sizeof(buf), "%09u", a[i]);
        s += buf;
    }
    return s;
}

// ---------- 交互 ----------
const int MAXLINE = 512; // 一行最多两个 101 位的编号

int queryCount = 0;

// 读一个非负十进制整数: 跳过空白, 读到下一个空白为止, 去掉多余的前导零
string readNumber(const char *line, int &p) {
    while (line[p] == ' ' || line[p] == '\r' || line[p] == '\n' || line[p] == '\t') p++;
    int st = p;
    while (line[p] >= '0' && line[p] <= '9') p++;
    int en = p;
    while (st + 1 < en && line[st] == '0') st++; // 归一化, 避免前导零影响判零
    if (st == en) return "0";
    return string(line + st, line + en);
}

// 询问编号为 node 的员工, 返回其左右下属的编号 (0 表示没有下属)
void ask(const string &node, string &lft, string &rgt) {
    printf("%s\n", node.c_str());
    fflush(stdout); // 交互题必须立刻把查询送出去, 否则双方互相等待
    queryCount++;
    char line[MAXLINE];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        lft = rgt = "0";
        return;
    }
    int p = 0;
    lft = readNumber(line, p);
    rgt = readNumber(line, p);
}

Big countFull(const string &node); // 完整统计 node 子树的员工数
Big countHalf(const string &node, const Big &x); // 已知子树大小是 x 或 x + 1

// 朴素统计: 递归问遍整棵子树
Big countFull(const string &node) {
    if (node == "0") return Big();
    string a, b;
    ask(node, a, b);
    Big rgt = countFull(b);      // 先把右子树数清楚
    Big lft = countHalf(a, rgt); // 左子树大小只能是 rgt 或 rgt + 1, 不必再全问一遍
    Big res = bigAdd(lft, rgt);
    return bigInc(res);
}

// 已知 node 子树的大小是 x 或 x + 1, 把它确定下来。每一步只需要再往一个方向递归:
// 设左右子树大小为 lft、rgt, 则 lft + rgt 是 x - 1 或 x, 且 lft 比 rgt 多 0 或 1。
//   x 为奇数时 lft + rgt 无论取 x-1(偶) 还是 x(奇), rgt 都被唯一确定为 floor(x/2);
//   x 为偶数时 lft 被唯一确定为 x/2, 于是 rgt 只能是 lft - 1 或 lft。
Big countHalf(const string &node, const Big &x) {
    if (node == "0") return Big();
    if (isZero(x)) return Big(1, 1); // 节点存在故大小非零, 只能是 1, 不必询问
    string a, b;
    ask(node, a, b);
    if (isOdd(x)) {
        Big rgt = bigHalve(x);        // = (x-1)/2
        Big lft = countHalf(a, rgt);  // lft 是 rgt 或 rgt + 1
        Big res = bigAdd(lft, rgt);
        return bigInc(res);
    }
    Big lft = bigHalve(x); // = x/2
    Big rgt = countHalf(b, bigDec(lft)); // rgt 是 lft - 1 或 lft
    Big res = bigAdd(lft, rgt);
    return bigInc(res);
}

int main() {
    Big ans = countFull("1"); // 根节点编号恒为 1
    printf("Answer %s\n", bigStr(ans).c_str());
    fflush(stdout);
    return 0;
}
