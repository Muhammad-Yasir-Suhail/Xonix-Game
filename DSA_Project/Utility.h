#pragma once
#include <string>
using namespace std;
int Length(const string& s);
string StringConverter(int n);
int StringToInt(string s);
template <typename T>
void Swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}