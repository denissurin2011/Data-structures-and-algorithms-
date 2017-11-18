#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

struct triple {
    int num;
    long long val;
    long long sum;
};

vector <triple> p;

void push(int i, long long m) {
    triple k;
    k.num = i;
    k.val = m;
    k.sum = p[i].sum + m;
    p.push_back(k);
}

void pop(int i) {
    p.push_back(p[p[i].num]);
}

int main() {
    int n;
    ifstream fin("snowmen.in");
    ofstream fout("snowmen.out");
    fin >> n;
    triple k;
    k.num = 0, k.sum = 0, k.val = 0;
    p.push_back(k);
    for (int i = 0; i < n; ++i) {
        int t;
        long long m;
        fin >> t >> m;
        if (m != 0) {
            push(t, m);
        } else {
            pop(t);
        }
    }
    long long ans = 0;
    for (auto i : p) {
        ans += i.sum;
    }
    fout << ans;
    fin.close();
    fout.close();
    return 0;
}
