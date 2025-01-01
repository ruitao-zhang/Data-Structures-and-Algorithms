#ifndef PIECEWISEPOLYNOMIAL_H
#define PIECEWISEPOLYNOMIAL_H

#include <vector>
#include <stdexcept>
#include <cmath>

// 定义分段多项式类（ppForm）
class PP {
private:
    std::vector<std::vector<double>> c; // 每段多项式的系数
    std::vector<double> b; // 分段点

public:
    // 构造函数：初始化分段点和系数
    PP(const std::vector<std::vector<double>>& c, const std::vector<double>& b)
        : c(c), b(b) {
        // 验证分段数是否匹配
        if (c.size() != b.size() - 1) {
            throw std::invalid_argument("Invalid");
        }
    }

    // 评估 ppForm 的值
    double f_4(double x) const {
        // 遍历分段点，找到对应的区间
        for (size_t i = 0; i < b.size() - 1; ++i) {
            if (x >= b[i] && x <= b[i + 1]) {
                double result = 0.0;
                double dx = x - b[i]; // x 相对于当前段起点的偏移

                // 使用多项式公式计算值：result = Σ c_j * dx^j
                for (size_t j = 0; j < c[i].size(); ++j) {
                    result += c[i][j] * std::pow(dx, j);
                }

                return result;
            }
        }

        // 如果超出范围，抛出异常
        throw std::out_of_range("Invalid");
    }
};

#endif // PIECEWISEPOLYNOMIAL_H

