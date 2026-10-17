#include <bits/stdc++.h>
using namespace std;
 
int main(){
    ios_base::sync_with_stdio(0); cin.tie(0);
    int n;
    cin >> n;
 
    vector<int> A(n), B(n);
 
    for (int &i : A)
        cin >> i;
    for (int &i : B)
        cin >> i;
    
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
        cout << i << "\n";
}