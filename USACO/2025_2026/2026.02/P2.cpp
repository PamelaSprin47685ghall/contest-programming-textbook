#include <bits/stdc++.h>
using namespace std; 
 
int main(){
    int n, k;
    cin >> n >> k;
 
    vector<vector<vector<int>>> isAt(n, vector<vector<int>>(n, vector<int>(n, 0)));
 
    for (int i = 0; i < k; i++) {
        int x, y, z;
        cin >> x >> y >> z;
        x--; y--; z--;
        isAt[x][y][z]++;
    }
 
    vector<int> score((1 << n), 0);
 
    for (int msk = 1; msk < (1 << n); msk++) {
        int msb = 31 - __builtin_clz(msk);
        vector<int> mpos, opos;
 
        for (int i = 0; i < n; i++) {
            if ((1 << i) & msk)
                mpos.push_back(i);
            else
                opos.push_back(i);
        }
 
        // Previously
        score[msk] = score[msk ^ (1 << msb)];
 
        // Turned bad from flipping MSB
        for (int m : mpos) 
            for (int o : opos)
                score[msk] -= isAt[m][msb][o] + isAt[m][o][msb];
        
        // Turned good from flipping MSB
        for (int o1 : opos)
            for (int o2 : opos)
                score[msk] += isAt[msb][o1][o2];
    }
    int bestVal = *max_element(score.begin(), score.end());
    int bestCount = count(score.begin(), score.end(), bestVal);
 
    cout << bestVal << " " << bestCount << "\n";
}