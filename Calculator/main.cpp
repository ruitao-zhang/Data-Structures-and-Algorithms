#include "expression_evaluator.h"
int main() {
    string expression;
    cout << "请输入一个数学表达式: ";
    getline(cin, expression);  // 获取包含空格的输入3b

    try {
        double result = evaluate(expression);
        cout << "结果: " << result << endl;
    } catch (const runtime_error& e) {
        cout << "ILLEGAL" << endl;
    }

    return 0;
}

