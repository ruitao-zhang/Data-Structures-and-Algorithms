#include "expression_evaluator.h"
#include <iostream>
#include <stack>
#include <cctype>
#include <stdexcept>
#include <string>
#include <cmath>
using namespace std;

int main() {
    string a;
    cout << "计算：";
    getline(cin, a);  

    try {
        double b = calculate(a);
        cout << "结果: " << b << endl;
    } catch (const runtime_error& e) {
        cout << "ILLEGAL" << endl;
    }

    return 0;
}
