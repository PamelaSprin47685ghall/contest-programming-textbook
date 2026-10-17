#include <bits/stdc++.h>
using namespace std;
 
int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); 
    int n, f;
    string S;
    cin >> n >> f >> S;
 
    map<string, int> occ;
    set<string> ans;
 
    auto chk = [&](int i){
        if (i < 0 or i + 2 >= n)
            return false;
            
        return (S[i] != S[i + 1] and S[i + 1] == S[i + 2]);
    };
    auto updPoint = [&](int i, int delta){
        if (!chk(i))
            return;
        
        string t = S.substr(i, 3);
 
        occ[t] += delta;
 
        if (occ[t] >= f)
            ans.insert(t);
    };
    auto updRange = [&](int i, int delta){
        updPoint(i - 2, delta);
        updPoint(i - 1, delta);
        updPoint(i, delta);
    };
 
    for (int i = 0; i < n; i++)
        updPoint(i, 1);
    
    for (int i = 0; i < n; i++){
        char temp = S[i];
 
        for (char c = 'a'; c <= 'z'; c++){
            updRange(i, -1);
            S[i] = c;
            updRange(i, 1);
        }
        updRange(i, -1);
        S[i] = temp;
        updRange(i, 1);
    }
    
    cout << ans.size() << "\n";
 
    for (auto s : ans)
        cout << s << "\n";
}