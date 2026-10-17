#include <cstdio>
#include <cstring>
#include <map>
#include <string>

using namespace std;

const long long MOD = 1000000007LL;   // 题目规定所有加法都在 10^9+7 下进行
const int MAXV = 105;                 // 变量数上限：100 行的程序最多定义 99 个变量
const int MAXS = 105;                 // 语句数上限
const int MAXD = 64;                  // MOO 嵌套层数上限

struct Stmt {
    int kind;               // 0 表示赋值，1 表示 MOO 循环
    int varId;              // 赋值语句的目标变量
    long long coef[MAXV];   // 右侧表达式：coef[0] 是常数项，coef[1+v] 是变量 v 的系数
    long long times;        // 循环次数
    int bodyEnd;            // 循环体（按前序存放）是 [本语句下标 + 1, bodyEnd)
};

Stmt stmts[MAXS];
int stmtCount = 0;
int varCount = 0;              // 已经出现过的变量个数
map<string, int> varId;        // 变量名到编号
int returnVar = -1;            // RETURN 的变量编号
char token[512];

long long exprCoef[MAXV];      // 解析表达式时使用的累加器

// 状态向量的第 0 维是恒等于 1 的"常量"，第 1+v 维是变量 v 的值。
// 每条语句、每个循环体都对应一个方阵：先执行 A 再执行 B 就是矩阵 B*A
long long accStack[MAXD][MAXV][MAXV];   // 显式栈：每层正在拼装的语句列表的累积矩阵
long long assignMat[MAXV][MAXV];        // 单条赋值语句的矩阵
long long loopMat[MAXV][MAXV];          // 循环体的矩阵的若干次幂
long long prodMat[MAXV][MAXV];          // 乘法暂存
int openBodyEnd[MAXD];                  // 各层循环体的结束下标
long long openTimes[MAXD];              // 各层的循环次数

bool nextToken() {
    return scanf("%s", token) == 1;
}

long long literalValue(const char *s) {
    long long value = 0;
    for (int i = 0; s[i]; ++i) value = value * 10 + (s[i] - '0');
    return value;
}

// 变量名统一在这里换编号；题目保证变量不会在定义前被使用
int idOf(const char *name) {
    map<string, int>::iterator it = varId.find(name);
    if (it == varId.end()) {
        varId[name] = varCount;
        return varCount++;
    }
    return it->second;
}

// 解析一个表达式，把各项分别累加到 exprCoef 里
void parseExpr() {
    nextToken();
    if (token[0] == '(') {
        parseExpr();     // 左子表达式
        nextToken();     // ")"
        nextToken();     // "+"
        nextToken();     // "("
        parseExpr();     // 右子表达式
        nextToken();     // ")"
    } else if (token[0] >= '0' && token[0] <= '9') {
        exprCoef[0] = (exprCoef[0] + literalValue(token)) % MOD;
    } else {
        int idx = 1 + idOf(token);
        exprCoef[idx] = (exprCoef[idx] + 1) % MOD;
    }
}

// 解析一个语句列表，读到 "}"（循环体结束）或 RETURN（程序结束）为止。
// 语句按前序编号：循环语句自己先占一个下标，随后才是它的循环体，
// 于是循环体是一段连续区间，处理时可以只看区间而不用递归
void parseStatements() {
    while (nextToken()) {
        if (token[0] == '}') return;
        if (token[0] == 'R') {
            nextToken();
            returnVar = idOf(token);
            return;
        }
        if (token[0] >= '0' && token[0] <= '9') {
            long long times = literalValue(token);
            nextToken();     // MOO
            nextToken();     // {
            int idx = stmtCount++;
            stmts[idx].kind = 1;
            stmts[idx].times = times;
            parseStatements();
            stmts[idx].bodyEnd = stmtCount;
        } else {
            int id = idOf(token);
            nextToken();     // "="
            memset(exprCoef, 0, sizeof(exprCoef));
            parseExpr();
            int idx = stmtCount++;
            stmts[idx].kind = 0;
            stmts[idx].varId = id;
            memcpy(stmts[idx].coef, exprCoef, sizeof(exprCoef));
        }
    }
}

void setIdentity(long long M[MAXV][MAXV], int dim) {
    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j) M[i][j] = (i == j) ? 1 : 0;
}

void copyMat(const long long A[MAXV][MAXV], long long B[MAXV][MAXV], int dim) {
    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j) B[i][j] = A[i][j];
}

// C = A * B（模 MOD 意义下）
void matMul(const long long A[MAXV][MAXV], const long long B[MAXV][MAXV],
            long long C[MAXV][MAXV], int dim) {
    for (int i = 0; i < dim; ++i) {
        long long *ci = C[i];
        for (int j = 0; j < dim; ++j) ci[j] = 0;
        for (int k = 0; k < dim; ++k) {
            long long a = A[i][k];
            if (a != 0) {
                const long long *bk = B[k];
                for (int j = 0; j < dim; ++j) ci[j] += a * bk[j];
            }
            // 每累加 8 项取一次模：8 * (10^9+6)^2 仍在 long long 范围内
            if ((k & 7) == 7)
                for (int j = 0; j < dim; ++j) ci[j] %= MOD;
        }
        for (int j = 0; j < dim; ++j) ci[j] %= MOD;
    }
}

// R = base^e（矩阵快速幂，e >= 1）
void matPow(const long long base[MAXV][MAXV], long long e,
            long long R[MAXV][MAXV], int dim) {
    static long long cur[MAXV][MAXV], tmp[MAXV][MAXV];
    setIdentity(R, dim);
    copyMat(base, cur, dim);
    while (e > 0) {
        if (e & 1) {
            matMul(cur, R, tmp, dim);
            copyMat(tmp, R, dim);
        }
        e >>= 1;
        if (e > 0) {
            matMul(cur, cur, tmp, dim);
            copyMat(tmp, cur, dim);
        }
    }
}

int main() {
    parseStatements();

    int dim = varCount + 1;   // 常量 + 所有变量
    setIdentity(accStack[0], dim);
    int depth = 0;
    for (int i = 0; i <= stmtCount; ++i) {
        // 本层循环体在这里结束：把它的矩阵取 times 次幂，再乘到外层的累积矩阵上
        while (depth > 0 && openBodyEnd[depth] == i) {
            matPow(accStack[depth], openTimes[depth], loopMat, dim);
            matMul(loopMat, accStack[depth - 1], prodMat, dim);
            copyMat(prodMat, accStack[depth - 1], dim);
            --depth;
        }
        if (i == stmtCount) break;
        if (stmts[i].kind == 0) {
            setIdentity(assignMat, dim);
            for (int j = 0; j < dim; ++j) assignMat[1 + stmts[i].varId][j] = stmts[i].coef[j];
            matMul(assignMat, accStack[depth], prodMat, dim);
            copyMat(prodMat, accStack[depth], dim);
        } else {
            ++depth;
            setIdentity(accStack[depth], dim);
            openBodyEnd[depth] = stmts[i].bodyEnd;
            openTimes[depth] = stmts[i].times;
        }
    }

    // 初始时所有变量都还没定义，取值无关紧要；把矩阵乘到"常量 1、变量全 0"的状态上，
    // 答案就是 RETURN 变量那一行在第 0 列（常量列）的元素
    printf("%lld\n", accStack[0][1 + returnVar][0]);
    return 0;
}
