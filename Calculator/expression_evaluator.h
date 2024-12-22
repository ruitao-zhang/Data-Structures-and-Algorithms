#include <iostream>
#include <stack>
#include <cctype>
#include <stdexcept>
#include <string>
#include <cmath>
using namespace std;

// 设置运算符优先级
int getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// 四则运算
double applyOperation(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) throw runtime_error("错误: 除数不能为零!");
            return a / b;
        default: throw runtime_error("无效的操作符");
    }
}

// 检查表达式是否合法
void validateExpression(const string& expression) {
    int openParenCount = 0;  // 记录左括号数量
    int closeParenCount = 0; // 记录右括号数量

    // 是否为空
    if (expression.empty()) {
        throw runtime_error("错误: 表达式为空!");
    }

    // 不能以运算符开头（但可以以左括号开头）
    if (expression[0] == '+' || expression[0] == '-' || expression[0] == '*' || expression[0] == '/') {
        throw runtime_error("错误: 表达式不能以运算符开头!");
    }

    // 不能以运算符结尾（但可以以右括号结尾）
    if (expression[expression.size() - 1] == '+' || expression[expression.size() - 1] == '-' || 
        expression[expression.size() - 1] == '*' || expression[expression.size() - 1] == '/') {
        throw runtime_error("错误: 表达式不能以运算符结尾!");
    }

    // 括号数目是否匹配及空括号
    for (size_t i = 0; i < expression.size(); ++i) {
        if (expression[i] == '(') {
            // 左括号后不能是运算符
            if (i + 1 < expression.size() && (expression[i + 1] == '+' || expression[i + 1] == '-' || 
                                               expression[i + 1] == '*' || expression[i + 1] == '/')) {
                throw runtime_error("错误: 左括号后不能跟运算符!");
            }
        }
        if (expression[i] == ')') {
            // 右括号前不能是运算符
            if (i > 0 && (expression[i - 1] == '+' || expression[i - 1] == '-' || 
                          expression[i - 1] == '*' || expression[i - 1] == '/')) {
                throw runtime_error("错误: 右括号前不能跟运算符!");
            }
        }
    }

    // 左右括号数量是否匹配
    if (openParenCount != closeParenCount) {
        throw runtime_error("错误: 括号数量不匹配!");
    }

    // 是否除数为零
    for (size_t i = 0; i < expression.size(); ++i) {
        if (expression[i] == '/' && i + 1 < expression.size() && expression[i + 1] == '0') {
            throw runtime_error("错误: 除数不能为零!");
        }
    }

    // 运算符是否连续出现
    for (size_t i = 1; i < expression.size(); ++i) {
        if ((expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') && 
            (expression[i - 1] == '+' || expression[i - 1] == '-' || expression[i - 1] == '*' || expression[i - 1] == '/')) {
            throw runtime_error("错误: 运算符不能连续出现!");
        }
    }
}

// 计算中缀表达式
double evaluate(const string& expression) {
    // 检验是否合法
    validateExpression(expression);

    stack<double> values;  // 存储操作数
    stack<char> ops;       // 存储运算符

    for (size_t i = 0; i < expression.size(); ++i) {
        if (isspace(expression[i])) {
            continue;  // 跳过空格
        }

        if (isdigit(expression[i]) || expression[i] == '.') {
            // 如果是数字或小数点，读取完整的数字并转换为浮点数
            double val = 0;
            bool hasDecimal = false;
            double decimalPlace = 1;

            while (i < expression.size() && (isdigit(expression[i]) || expression[i] == '.')) {
                if (expression[i] == '.') {
                    if (hasDecimal) {
                        throw runtime_error("错误: 不允许多个小数点");
                    }
                    hasDecimal = true;
                } else {
                    val = val * 10 + (expression[i] - '0');
                    if (hasDecimal) {
                        decimalPlace *= 10;
                    }
                }
                ++i;
            }

            if (hasDecimal) {
                val /= decimalPlace;  // 调整小数点后的数字
            }
            values.push(val);
            --i;  // 因为循环会跳过一个字符，需要回退
        } else if (expression[i] == '(') {
            // 左括号，压栈
            ops.push(expression[i]);
        } else if (expression[i] == ')') {
            // 右括号，计算直到遇到左括号
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(val1, val2, op));
            }
            ops.pop();  // 弹出 '('
        } else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            // 运算符，先处理栈中的运算符
            while (!ops.empty() && getPrecedence(ops.top()) >= getPrecedence(expression[i])) {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(val1, val2, op));
            }
            // 当前运算符压栈
            ops.push(expression[i]);
        }
    }

    // 处理剩余的运算符
    while (!ops.empty()) {
        double val2 = values.top(); values.pop();
        double val1 = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOperation(val1, val2, op));
    }

    // 最终结果在栈顶
    return values.top();
}
