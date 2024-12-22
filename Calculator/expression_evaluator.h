#include <iostream>
#include <stack>
#include <cctype>
#include <stdexcept>
#include <string>
#include <cmath>
using namespace std;

// 设置运算符优先级
int set(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// 四则运算
double rule(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) throw runtime_error("");
            return a / b;
        default: throw runtime_error("");
    }
}

// 检查表达式是否合法
void judge(const string& a) {
    int left = 0;  // 左括号数量
    int right = 0; // 右括号数量

    // 是否为空
    if (a.empty()) {
        throw runtime_error("");
    }

    // 不能以运算符开头（但可以以左括号开头）
    if (a[0] == '+' || a[0] == '-' || a[0] == '*' || a[0] == '/') {
        throw runtime_error("");
    }

    // 不能以运算符结尾（但可以以右括号结尾）
    if (a[a.size() - 1] == '+' || a[a.size() - 1] == '-' || 
        a[a.size() - 1] == '*' || a[a.size() - 1] == '/') {
        throw runtime_error("");
    }

    // 括号数目是否匹配及空括号处理 
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] == '(') {
            // 左括号后不能是运算符
            left++;
            if (i + 1 < a.size() && (a[i + 1] == '+' || a[i + 1] == '-' || 
                a[i + 1] == '*' || a[i + 1] == '/' || a[i+1] == ')')) {
                throw runtime_error("");
            }
        }
        if (a[i] == ')') {
            // 右括号前不能是运算符
            right++;
            if (i > 0 && (a[i - 1] == '+' || a[i - 1] == '-' || 
                          a[i - 1] == '*' || a[i - 1] == '/' || a[i+1] == '(')) {
                throw runtime_error("");
            }
        }
    }

    // 左右括号数量是否匹配
    if (left != right) {
        throw runtime_error("");
    }

    // 是否除数为零
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] == '/' && i + 1 < a.size() && a[i + 1] == '0') {
            throw runtime_error("");
        }
    }

    // 运算符是否连续出现
    for (size_t i = 1; i < a.size(); ++i) {
        if ((a[i] == '+' || a[i] == '-' || a[i] == '*' || a[i] == '/') && 
            (a[i - 1] == '+' || a[i - 1] == '-' || a[i - 1] == '*' || a[i - 1] == '/')) {
            throw runtime_error("");
        }
    }
}

// 计算中缀表达式
double calculate(const string& a) {
    // 检验是否合法
    judge(a);

    stack<double> num;  // 存储操作数
    stack<char> opt;       // 存储运算符

    for (size_t i = 0; i < a.size(); ++i) {
        if (isspace(a[i])) {
            continue;  // 跳过空格
        }

        if (isdigit(a[i]) || a[i] == '.') {
            // 如果是数字或小数点，读取完整的数字并转换为浮点数
            double m = 0;
            bool k = false;
            double n = 1;

            while (i < a.size() && (isdigit(a[i]) || a[i] == '.')) {
                if (a[i] == '.') {
                    if (k) {
                        throw runtime_error("");
                    }
                    k = true;
                } else {
                    m = m * 10 + (a[i] - '0');
                    if (k) {
                        n *= 10;
                    }
                }
                ++i;
            }

            if (k) {
                m /= n;  // 调整小数点后的数字
            }
            num.push(m);
            --i;  // 因为循环会跳过一个字符，需要回退
        } else if (a[i] == '(') {
            // 左括号，压栈
            opt.push(a[i]);
        } else if (a[i] == ')') {
            // 右括号，计算直到遇到左括号
            while (!opt.empty() && opt.top() != '(') {
                double num2 = num.top(); num.pop();
                double num1 = num.top(); num.pop();
                char op = opt.top(); opt.pop();
                num.push(rule(num1, num2, op));
            }
            opt.pop();  // 弹出 '('
        } else if (a[i] == '+' || a[i] == '-' || a[i] == '*' || a[i] == '/') {
            // 运算符，先处理栈中的运算符
            while (!opt.empty() && set(opt.top()) >= set(a[i])) {
                double num2 = num.top(); num.pop();
                double num1 = num.top(); num.pop();
                char op = opt.top(); opt.pop();
                num.push(rule(num1, num2, op));
            }
            // 当前运算符压栈
            opt.push(a[i]);
        }
    }

    // 处理剩余的运算符
    while (!opt.empty()) {
        double num2 = num.top(); num.pop();
        double num1 = num.top(); num.pop();
        char op = opt.top(); opt.pop();
        num.push(rule(num1, num2, op));
    }

    // 最终结果在栈顶
    return num.top();
}
