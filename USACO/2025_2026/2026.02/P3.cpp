#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <algorithm>
using namespace std;

void solve()
{
    int N, K, Q; (void)K;
    scanf("%d %d", &N, &Q);
 
    int price[N];
    scanf("%d", &price[0]);
    for (int idx = 1; idx < N; ++idx)
    {
        scanf("%d", &price[idx]);
        price[idx] = min(price[idx], 2 * price[idx-1]);
    }
 
    for (int idx = 0; idx < Q; ++idx)
    {
        int x;
        scanf("%d", &x);
 
        long long minCost = 1e18;
        long long curCost = 0;
        for (int jdx = min(30, N-1); jdx >= 0; --jdx)
        {
            int curBuckets = 1<<jdx;
            long long count = x / curBuckets;
 
            x -= count*curBuckets;
            curCost += count*price[jdx];
 
            if (x == 0)
                minCost = min(minCost, curCost);
            else
                minCost = min(minCost, curCost + price[jdx]);
        }
 
        printf("%lld\n", minCost);
    }
}
 
int main()
{
    solve();
 
    return 0;
}