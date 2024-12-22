#include "expression_evaluator.h"
#include <iostream>
#include <stack>
#include <cctype>
#include <stdexcept>
#include <string>
#include <cmath>
using namespace std;

int main() {
    string expression;
    cout << "计算: ";
    getline(cin, expression);  

    try {
        double result = evaluate(expression);
        cout << "结果: " << result << endl;
    } catch (const runtime_error& e) {
        cout << "ILLEGAL" << endl;
    }

    return 0;
}

