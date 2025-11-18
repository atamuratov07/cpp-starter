#include "bits/stdc++.h"
using namespace std;

// Type aliases
using ll = long long;
using ld = long double;
using vi = vector<int>;
using vl = vector<ll>;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

// Macros
#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define each(e, v) for (auto &e : (v))
#define rep(i, n) for (int i = 0; i < (n); ++i)
#define repb(i, n) for (int i = (n) - 1; i >= 0; --i)

// Constants
const int INF = 1e9;
const ll LINF = 1e18;
const int MOD = 1e9 + 7;

// Utility functions
template<class T> inline bool minimize(T &a, const T &b){ return b<a ? (a=b,1):0; }
template<class T> inline bool maximize(T &a, const T &b){ return b>a ? (a=b,1):0; }

// I/O utilities
template<typename T>
void read(T& x) { cin >> x; }

template<typename T>
void read(vector<T>& v) {
    for (auto& x : v) cin >> x;
}

template<typename T, typename... Args>
void read(T& first, Args&... args) {
    read(first);
    read(args...);
}

template<typename T>
void print(const T& x) { cout << x << '\n'; }

template<typename T>
void print(const vector<T>& v) {
    for (int i = 0; i < sz(v); ++i) {
        cout << v[i];
        if (i < sz(v) - 1) cout << ' ';
    }
    cout << '\n';
}

template<typename T, typename... Args>
void print(const T& first, const Args&... args) {
    cout << first;
    ((cout << ' ' << args), ...);
    cout << '\n';
}

// ==============================================

void solve() {
   int n;
   read(n);
   print(n);
}

// ==============================================

int main() {
   ios::sync_with_stdio(false);
   cin.tie(nullptr);

   int t = 1;
   // read(t);

   while (t--) {
      solve();
   }

   return 0;
}