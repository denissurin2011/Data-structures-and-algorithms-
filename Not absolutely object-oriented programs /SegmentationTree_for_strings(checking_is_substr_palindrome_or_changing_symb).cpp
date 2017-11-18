#include<cstdio>
#include<iostream>
#include<vector>
#include<cstdlib>
#include<cstddef>
#include<fstream>
#include<set>
#include <ctime>
#include <string>
#include <algorithm>

using namespace std;

vector <long long> p;
vector<long long> pr;
int M = 1e9 + 7;
long long p1 = 107;

inline long long gc(char c) {
    return c - 'a' + 1;
}

void build(long long a[], int v, int tl, int tr, long long t[]) {
    if (tl + 1 == tr) {
        t[v] = a[tl];
    } else {
        build(a, 2*v, tl, (tl + tr)/2, t);
        build(a, 2*v + 1, (tl + tr)/2,  tr, t);
        t[v] = (t[2*v] + (t[2*v+1]*p[(tr - tl) / 2]%M)) % M;
    }
}

long long sum(long long a[], long long t[], int v, int tl, int tr, int l, int r) {
    if (l <= tl && tr <= r) {
        return (t[v]*p[tl - l])%M;
    } else {
        if (tr <= l || tl >= r) {
            return 0;
        } else {
            int m = (tl + tr)/2;
            return (sum(a, t, 2*v, tl, m, l, r) + sum(a, t, 2*v + 1, m, tr, l, r)) % M;
        }
    }
}

void change(long long t[], int k, int x, int l, int r, int v) {
    if (r - l == 1) {
        t[v] = x;
    } else {
        int m = (r + l) / 2;
        if (k > m) {
            change(t, k, x, m, r, 2*v + 1);
        } else {
            change(t, k, x, l, m, 2 * v);
        }
        t[v] = (t[2*v] + (t[2*v + 1]*p[(r - l) / 2])%M) % M;
    }
}

int main() {
    freopen("substring-palindromes.in", "rt", stdin);
    freopen("substring-palindromes.out", "wt", stdout);
    bool l1 = true;
    string s;
    cin >> s;
    int m;
    cin >> m;
    p.resize(s.size() + 1);
    p[0] = 1;
    long long p2 = p1;
    for (int i = 1; i <= s.size(); i++) {
        p[i] = p2;
        p2 = (p2*p1) % M;
    }
    long long a_r[s.size()];
    int n = s.size();
    long long t[4*n];
    long long t_r[4*n];
    long long a[s.size()];
    string s1 = s;
    reverse(s1.begin(), s1.end());
    for (int i = 0; i < n; i++) {
        a[i] = gc(s[i]);
        a_r[i] = gc(s1[i]);
    }
    build(a, 1, 0, n, t);
    build(a_r, 1, 0, n, t_r);
    for (int i = 0; i < m; i++) {
        string s2;
        cin >> s2;
        int l, r;
        int l1, r1;
        if (s2[s2.size() - 1] == '?') {
            cin >> l >> r;
            r1 = n - l + 1;
            l1 = n - r + 1;
            if (sum(a, t, 1, 0, n, l-1, r) == sum(a_r, t_r, 1, 0, n, l1-1, r1)) {
                cout << "Yes" << endl;
            } else {
                cout << "No" << endl;
            }
        } else {
            int pos;
            char c;
            cin >> pos >> c;
            int x = gc(c);
            change(t, pos, x, 0, n, 1);
            change(t_r, n - pos + 1, x, 0, n, 1);
        }
    }
    return 0;
}
