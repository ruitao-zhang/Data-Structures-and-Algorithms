#ifndef BSPLINECURVE_H
#define BSPLINECURVE_H

#include "BSplineBasis.h"
#include <vector>

// 定义 B-Spline 曲线类
class BSplineCurve {
private:
    BSplineBasis basis; // B-Spline 基函数对象
    std::vector<double> c; // 权重

public:
    // 初始化 B-Spline 基函数和权重
    BSplineCurve(const BSplineBasis& basis, const std::vector<double>& c)
        : basis(basis), c(c) {
        // 验证数量是否与 B-Spline 基函数匹配
        if (c.size() + basis.getDegree() + 1 != basis.getKnots().size()) {
            throw std::invalid_argument("Invalid");
        }
    }

    // 计算曲线在参数 t 下的值 S(t)
    double f_1(double t) const {
        double result = 0.0;

        // 计算 S(t) = Σ a_i * B_i^n(t)
        for (size_t i = 0; i < c.size(); ++i) {
            result += c[i] * basis.f_0(i, basis.getDegree(), t);
        }

        return result;
    }
};

#endif // BSPLINECURVE_H

