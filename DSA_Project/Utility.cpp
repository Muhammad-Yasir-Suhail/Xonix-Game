#include "Utility.h"

int Length(const string& s) {
    int len = 0;
    while (s[len] != '\0') len++;
    return len;
}
int StringToInt(string s) {
    int num = 0;
    bool isNegative = false;
    int i = 0;
    if (s[0] == '-') {
        isNegative = true;
        i = 1;
    }
    while (i < Length(s)) {
        num = num * 10 + (s[i] - '0');
        i++;
    }
    if (isNegative == true) {
        return -num;
    }
    else {
        return num;
    }
}
string StringConverter(int n) {
    if (n == 0) return "0";

    bool neg = false;
    if (n < 0) {
        neg = true;
        n = -n;
    }
    char temp[20];
    int i = 0;
    while (n > 0) {
        temp[i++] = (n % 10) + '0';
        n /= 10;
    }
    string res = "";
    if (neg) {
        res += '-';
    }
    for (int j = i - 1; j >= 0; j--) {
        res += temp[j];
    }
    return res;
}
