#include <cstdio>
#include <vector>

using namespace std;

int main(){
    int n;
    scanf("%d", &n);

    vector<int> A(n), B(n);

    for (int &i : A)
        scanf("%d", &i);
    for (int &i : B)
        scanf("%d", &i);

    int alreadySame = 0;
    vector<int> ans(n + 1, 0);

    for (int i = 0; i < n; i++)
        alreadySame += (A[i] == B[i]);

    auto expand = [&](int l, int r){
        int match = alreadySame;

        for (; l >= 0 and r < n; l--, r++){
            match += ((A[l] == B[r]) + (A[r] == B[l])) - ((A[l] == B[l]) + (A[r] == B[r]));
            ans[match]++;
        }
    };
    for (int mid = 0; mid < n; mid++){
        expand(mid, mid);
        expand(mid, mid + 1);
    }
    for (int i : ans)
        printf("%d\n", i);
}
