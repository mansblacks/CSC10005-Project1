#include "QFloat.h"

bool compare_to_one(std::string value) { // true when higher, false when lower
	return value[0] > '0';
}

std::string _two_power_minus(const int& exp) {
    QInt p("1", 10);

    for (int i = 0; i < exp; i++) {
        p = p * QInt("5", 10);
    }

    std::string answer = p.print_QInt(10);
    remove_leading_zeroes(answer);

    for (int i = 0; i < answer.length() - exp; i++) {
        answer = '0' + answer;
    }

    answer = "0." + answer;

    return answer;
}

std::string _string_fraction_addition(std::string a, std::string b) {
    while (a.length() < b.length()) {
        a += '0';
    }

    while (b.length() < a.length()) {
        b += '0';
    }

    int t = 1000;
    int cnt = 0;
    while (a.front() == '0' || a.front() == '.') {
        a.erase(a.begin());
        cnt++;
    }
    t = std::min(t, cnt);
    cnt = 0;

    while (b.front() == '0' || b.front() == '.') {
        b.erase(b.begin());
        cnt++;
    }
    t = std::min(t, cnt);

    QInt ans = QInt(a, 10) + QInt(b, 10);

    return "0." + std::string(t - 2, '0') + ans.print_QInt(10);
}

std::string _string_mul_two(std::string value) {
	int point = 0;
	std::string tmp = "", res = "";

	for (size_t i = 0; i < value.length(); ++i) {
		if (value[i] == '.') {
			point = i;
			continue;
		}
		tmp += value[i];
	}

	int r = 0, q = 0;

	for (int i = tmp.length() - 1; i >= 0; i--) {
		q = (tmp[i] - '0') * 2 + r;
		r = q / 10;
		q = q % 10;
		res += (q + '0');
	}

	if (r != 0) {
		res += (r + '0');
		point++;
	}

	std::reverse(res.rbegin(), res.rend());
	value = "";

	for (size_t i = 0; i < res.length();) {
		if (i == point) {
			value += '.';
			point = -1;
			continue;
		}
		value += res[i];
		i++;
	}

	return value;
}

std::string _string_minus_one(std::string value) { // value > 1
	remove_leading_zeroes(value);
    value[0] = '0';
	return value;
}

std::string _mantissa_padding(std::string st) {
    while (st.length() < _FP_MANTISSA) {
        st = st + '0';
    }
    return st;
}

std::string QFloat::dec_to_bin(std::string value) {
    int sign = value[0] == '-';

    if (value[0] == '-') {
        value.erase(value.begin());
    }

    remove_leading_zeroes(value);

    if (value == "0") {
        return std::string(_FP_LENGTH, '0');
    }

    std::string s = "0";
    for (int i = 0; i < value.length(); i++) {
        if (value[i] == '.') {
            break;
        }
        s += value[i];
    }

    remove_leading_zeroes(s);

    s = QInt::dec_to_bin(s);

	std::string res = "";
    value.erase(value.begin(), value.begin() + value.find('.'));
    value = '0' + value;
	for (int i = _FP_LENGTH - 1; i >= 0; i--) {
		if (compare_to_one(_string_mul_two(value))) {
			value = _string_minus_one(_string_mul_two(value));
			res += "1";
		}
		else {
			value = _string_mul_two(value);
			res += "0";
		}
	}

    res = s + '.' + res;

    unsigned long long bias = (int) pow(2, _FP_EXPONENT - 1) - 1;

    int exp = 0;
    int pos = res.find('.');

    while (pos != 1) {
        res[pos] = res[pos - 1];
        res[pos - 1] = '.';
        pos--;
        exp++;
    }

    while (res[pos - 1] != '1') {
        res[pos] = res[pos + 1];
        res[pos + 1] = '.';
        pos++;
        exp--;
    }

    exp += bias;

	return (sign ? '1' : '0') + std::bitset<_FP_EXPONENT>(exp).to_string() + _mantissa_padding(res.substr(pos + 1, _FP_MANTISSA));
}

std::string QFloat::bin_to_dec(std::string value) {
    int sign = value[0] == '1';
    unsigned long long bias = (int) pow(2, _FP_EXPONENT - 1) - 1;
    unsigned long long exp = std::bitset<_FP_EXPONENT>(value.substr(1, _FP_EXPONENT)).to_ullong() - bias;

    std::string mantissa = "1." + value.substr(_FP_EXPONENT + 1, _FP_MANTISSA);

    std::string man = "0.0";
    for (int i = 2; i < mantissa.length(); i++) {
        if (mantissa[i] == '1') {
            man = _string_fraction_addition(man, _two_power_minus(i - 1));
        }
    }

    int pos = 1;
    for (; exp < 0; exp++) {
        mantissa[pos] = mantissa[pos - 1];
        mantissa[pos - 1] = '.';
        pos--;
        if (pos == 0) {
            mantissa = '0' + mantissa;
            pos = 1;
        }
    }

    for (; exp > 0; exp--) {
        mantissa[pos] = mantissa[pos + 1];
        mantissa[pos + 1] = '.';
        pos++;
        if (pos == mantissa.length() - 1) {
            mantissa = mantissa + '0';
        }
    }

    std::string dec = QInt::bin_to_dec(mantissa.substr(0, mantissa.find('.')));
    std::string frac = mantissa.substr(mantissa.find('.') + 1, mantissa.length() - mantissa.find('.') - 1);

    remove_trailing_zeroes(frac);
    std::string fraction = "0.0";
    for (int i = 0; i < frac.length(); i++) {
        if (frac[i] == '1') {
            fraction = _string_fraction_addition(fraction, _two_power_minus(i + 1));
        }
    }

    return dec + fraction.substr(1, fraction.length() - 1);
}