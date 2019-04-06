#include <iostream>
#include <string>
#include <cctype>
using namespace std;

#include "QInt.h"
#include "QFloat.h"
#include "floating_binary.h"

bool is_nan(const string& st) {
    for (int i = 0; i < st.length(); i++) {
        if (!isdigit(st[i])) {
            return true;
        }
    }
    return false;
}

int main() {
    floating_binary a("00111110100000000000000000000000");
    floating_binary b("00111110100110011001100110011010");
    cout << (a / b).to_string() << endl;
    cout << QFloat::dec_to_bin("0.00007") << endl;
    cout << QFloat::bin_to_dec("01000001100010111011100111000001") << endl;
    return 0;
}
