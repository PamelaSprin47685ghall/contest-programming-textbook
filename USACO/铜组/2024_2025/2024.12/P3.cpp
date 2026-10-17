#include <cstdio>
#include <map>
#include <set>
#include <string>

using namespace std;

int main(){
    int n, f;
    scanf("%d%d", &n, &f);

    char buffer[20005];
    scanf("%s", buffer);
    string S = buffer;

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

    printf("%d\n", (int) ans.size());

    for (auto s : ans)
        printf("%s\n", s.c_str());
}
