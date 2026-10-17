#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<string> data(N);
    for (int i = 0; i < N; i++) {
        cin >> data[i];
    }
    
    vector<vector<int>> beat(N, vector<int>(N, 0));
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < i; j++) {
            if (data[i][j] != 'D') {
                if (data[i][j] == 'W')
                    beat[i][j] = 1;
                else
                    beat[j][i] = 1;
            }
        }
    }
    
    while (M--) {
        int x, y;
        cin >> x >> y;
        x--; 
        y--;
        
        // Number of symbols b that can beat both x and y
        int winning = 0;
        for (int b = 0; b < N; b++) {
            if (beat[b][x] && beat[b][y])
                winning++;
        }
        
        int ans = 0;
        for (int a = 0; a < N; a++) {
            if (beat[a][x] && beat[a][y]) {
                // Case 1: The symbol in Bessie's left hoof beats both symbols x and y
                // Bessie can play any symbol in her right hoof
                ans += N;
            } else {
                // Case 2: The symbol in Bessie's left hoof cannot beat both symbols x and y
                // Bessie must play a symbol that can beat both x and y in her right hoof
                ans += winning;
            }
        }
        
        cout << ans << "\n";
    }
    
    return 0;
}