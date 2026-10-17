#include "bits/extc++.h"
 
using namespace std;
 
using ll = long long;
 
#define sz(x) int((x).size())
 
constexpr ll mod = 1e9+7;
 
void solve(){
    string s;
    cin>>s;
    ll moves=0;
    int n=sz(s);
    ll tpow[n]{}; // powers of 2
    tpow[0]=1;
    for(int i = 1; i<n; i++){
        tpow[i]=(tpow[i-1]*2)%mod;
    }
    {
        // first make all digits 0 or 1
        bool all01=1;
        for(auto u: s){
            if(u!='0' && u!='1'){
                all01=0;
                break;
            }
        }
        if(!all01){
            moves++;
            for(auto &u: s){
                u=char(((u-'0')%2)+'0');
            }
        }
    }
    reverse(s.begin(),s.end());
    for(int i = 0; i<n; i++){
        if(s[i]=='1'){
            if(i==0)moves++;
            else{
                moves=(moves+tpow[i]+tpow[i-1])%mod;
            }
        }
    }
    cout<<moves<<"\n";
}
 
int main(){
    cin.tie(0)->sync_with_stdio(0);
    cin.exceptions(ios::failbit);
    int t;
    cin>>t;
    while(t--)solve();
}