#include<cstdio>
#include<iostream>
#include<vector>
#include<cstdlib>
#include<cstddef>
#include<fstream>
#include<set>
#include <ctime>
#include<algorithm>
#include<utility>

using namespace std;

ifstream fin("lca2.in");
ofstream fout("lca2.out");

vector<vector <int> > g;
vector<int> d;
vector<int> order;
vector<bool> used;
int n, m;
vector<int> first;

void deep(int v, int t) {
    d[v] = t;
    for (int i = 0; i < g[v].size(); i++) {
        if (d[g[v][i]] == -1) {
            deep(g[v][i], t + 1);
        }
    }
    return;
}

void dfs(int v) {
    used[v] = true;
    order.push_back(v);
    for (int i = 0; i < g[v].size(); i++) {
        if (!used[g[v][i]]) {
            dfs(g[v][i]);
            order.push_back(v);
        }
    }

    return;
}

int main() {
    fin >> n >> m;
    d.resize(n);
    g.resize(n);
    for (int i = 0; i < n; i++) {
        d[i] = -1;
    }
    for (int i = 1; i < n; i++) {
        int a;
        fin >> a;
        g[i].push_back(a);
        g[a].push_back(i);
    }
    deep(0, 0);
    used.resize(n);
    for (int i = 0; i < n; i++) {
        used[i] = false;
    }
    dfs(0);
    long long ans = 0;
    long long a1, a2, x, y, z;
    fin >> a1 >> a2 >> x >> y >> z;
    first.resize(n);
    for (int i = 0; i < n; i++) {
        first[i] = -1;
    }
    for (int i = 0; i < order.size(); i++) {
        if (first[order[i]] == -1) {
            first[order[i]] = i;
        }
    }
    int k = 0;
    vector<int> pow;
    for (int i = 1; i <= order.size(); i *= 2) {
        pow.push_back(i);
        k++;
    }
    vector<vector<pair<int, int> > > st;
    st.resize(k + 1);
    for (int i = 0; i <= k; i++) {
        for (int j = 0; j < order.size(); j++) {
            if (i == 0) {
                st[i].push_back(make_pair(d[order[j]], order[j]));
            } else {
                if (j + pow[i - 1] < st[i - 1].size()) {
                    st[i].push_back(min(st[i - 1][j], st[i - 1][j + pow[i - 1]]));
                }
            }
        }
    }
    vector<int> pow_2;
    pow_2.push_back(-1);
    pow_2.push_back(0);
    k = 1;
    for (int i = 2; i <= order.size(); i *= 2) {
        for (int j = i; j < i*2; j++) {
            pow_2.push_back(k);
        }
        k++;
    }
    long long a11 = a1;
    long long res = -1;
    for (int i = 0; i < m; i++) {
        if (res != -1) {
            a1 = (x*a1 + y*a2 + z) % n;
            a2 = (x*a2 + y*a1 + z) % n;
            a11 = (a1 + res) % n;
        }
        int l, r;
        l = min(first[a11], first[a2]);
        r = max(first[a11], first[a2]);

        if (l == r) {
            res = st[0][l].second;
            ans += res;
            continue;
        }
        if (abs(r - l) == 1) {
            res = min(st[0][l], st[0][r]).second;
            ans += res;
            continue;
        }
        k = pow_2[r - l + 1];
        res = min(st[k][l], st[k][r - pow[k] + 1]).second;
        ans += res;
    }
    fout << ans;
    fin.close();
    fout.close();
    return 0;
}
