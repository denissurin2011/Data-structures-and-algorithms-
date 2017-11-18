#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <complex>
#include <fstream>

using namespace std;
const double PI = 3.141592653;
typedef complex<double> base;

void FFT (vector<base>& fur_pol) {
    int len = (int) fur_pol.size();
    if (len == 1) {
        return;
    }
    vector<base> even_mult, odd_mult;
    even_mult.resize(len/2);
    odd_mult.resize(len/2);
    for (int i = 0, j = 0; i < len; i += 2, j++) {
        even_mult[j] = fur_pol[i];
    }
    for (int i = 1, j = 0; i < len; i += 2, j++) {
        odd_mult[j] = fur_pol[i];
    }
    FFT(even_mult);
    FFT(odd_mult);
    double ang = (2 * PI) / len;
    base wn(cos(ang), sin(ang));
    base w(1);
    for (int i = 0; i < len/2; i++) {
        fur_pol[i] = even_mult[i] + odd_mult[i] * w;
        w *= wn;
    }
    base w1(1);
    for (int i = len/2; i < len; i++) {
        fur_pol[i] = even_mult[i - len/2] - odd_mult[i - len/2] * w1;
        w1 *= wn;
    }
}

void IFFT(vector<base>& fur_pol) {
    int len = (int) fur_pol.size();
    if (len == 1) {
        return;
    }
    vector<base> even_mult, odd_mult;
    even_mult.resize(len/2);
    odd_mult.resize(len/2);
    for (int i = 0, j = 0; i < len; i += 2, j++) {
        even_mult[j] = fur_pol[i];
    }
    for (int i = 1, j = 0; i < len; i += 2, j++) {
        odd_mult[j] = fur_pol[i];
    }
    IFFT(even_mult);
    IFFT(odd_mult);
    double ang = (2 * PI) / len;
    ang *= -1;
    base wn(cos(ang), sin(ang));
    base w(1);
    for (int i = 0; i < len/2; i++) {
        fur_pol[i] = even_mult[i] + odd_mult[i] * w;
        w *= wn;
    }
    base w1(1);
    for (int i = len/2; i < len; i++) {
        fur_pol[i] = even_mult[i - len/2] - odd_mult[i - len/2] * w1;
        w1 *= wn;
    }
    for (int i = 0; i < len; i++) {
        fur_pol[i] /= 2;
    }
}

void Multiply(vector<int>& first_num, vector<int>& second_num, vector<int>& ans, int length) {
    vector<base> fur_pol_1(first_num.begin(), first_num.end());
    fur_pol_1.resize(length);
    vector<base> fur_pol_2(second_num.begin(), second_num.end());
    fur_pol_2.resize(length);
    FFT(fur_pol_1);
    FFT(fur_pol_2);
    vector<base> fur_pol_res(length);
    for (int i = 0; i < length; i++) {
        fur_pol_res[i] = fur_pol_1[i] * fur_pol_2[i];
    }
    IFFT(fur_pol_res);
    for (int i = 0; i < length; i++) {
        ans[i] = int(fur_pol_res[i].real() + 0.5);
    }
    int digit = 0;
    for (int i = 0; i < ans.size(); i++) {
        ans[i] += digit;
        digit = ans[i] / 10;
        ans[i] %= 10;
    }
}

int len(string& string1, string& string2) {
    int length = max(string1.size(), string2.size());
    vector<int> pow_two(20);
    int pow_cur = 1;
    for (int i = 0; i < 20; i++) {
        pow_two[i] = pow_cur;
        pow_cur *= 2;
    }
    for (int i = 1; i < 20; i++) {
        if (length <= pow_two[i] && length >= pow_two[i - 1]) {
            if (length == pow_two[i - 1]) {
                length = pow_two[i - 1];
            } else {
                length = pow_two[i];
            }
            break;
        }
    }
    return length;
}

bool sign(string& string1, string& string2) {
    int sign1 = 1, sign2 = 1;
    if (string1[0] == '-') {
        string1 =  string1.substr(1, string1.size() - 1);
        sign1 *= -1;
    }
    if (string2[0] == '-') {
        string2 =  string2.substr(1, string2.size() - 1);
        sign2 *= -1;
    }
    return (sign1 * sign2 < 0);
}

bool zero(string& string1, string& string2) {
    return (string1[0] == '0' || string2[0] == '0');
}

void init(string& string_in, vector<int>& out, int length) {
    reverse(string_in.begin(), string_in.end());
    for (int i = 0; i < length; i++) {
        if (i >= string_in.size()) {
            out[i] = 0;
        } else {
            char symb =  string_in[i];
            out[i] = symb - 48;
        }
    }
}

int main() {
    ifstream fin ("multiply.in");
    ofstream fout ("multiply.out");
    string string1, string2;
    fin >> string1 >> string2;
    int length = len(string1, string2);
    if (zero(string1, string2)) {
        fout << 0;
        return 0;
    }
    if(sign(string1, string2)) {
        fout << '-';
    }
    vector<int> first_num(length);
    vector<int> second_num(length);
    init(string1, first_num, length);
    init(string2, second_num, length);
    length *= 2;
    vector<int> ans(length);
    Multiply(first_num, second_num, ans, length);
    bool l = false;
    for (int i = length - 1; i >= 0; i--) {
        if (l) {
            fout << ans[i];
        } else {
            if (ans[i] == 0) {
                continue;
            } else {
                l = true;
                fout << ans[i];
            }
        }
    }
    fin.close();
    fout.close();
    return 0;
}
