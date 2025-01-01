#ifndef BSPLINEBASIS_H
#define BSPLINEBASIS_H

#include <vector>
#include <stdexcept>

// B-Spline 基函数类
class BSplineBasis {
private:
    std::vector<double> k; // 节点序列
    int degree; // B-Spline 的阶数

public:
    // 构造函数：初始化节点序列和阶数
    BSplineBasis(const std::vector<double>& k, int degree)
        : k(k), degree(degree) {
        // 至少 degree + 2 个节点
        if (knots.size() < degree + 2) {
            throw std::invalid_argument("Invalid");
        }
    }

    // 计算 B_i^n(t) 
    double f_0(int i, int n, double t) const {
        if (n == 0) {
            // 0 阶基函数：t 是否在 [k[i], k[i+1]) 区间内
            return (t >= k[i] && t < k[i + 1]) ? 1.0 : 0.0;
        }

        // 左项和右项的递归计算（使用分段定义）
        double left = 0.0, right = 0.0;

        // 计算左项：避免除零
        if (k[i + n] != k[i]) {
            left = (t - k[i]) / (k[i + n] - k[i]) * basisFunction(i, n - 1, t);
        }

        // 计算右项：避免除零
        if (k[i + n + 1] != k[i + 1]) {
            right = (k[i + n + 1] - t) / (k[i + n + 1] - k[i + 1]) * basisFunction(i + 1, n - 1, t);
        }

        // 返回递归计算结果
        return left + right;
    }

    // 获取节点序列
    const std::vector<double>& getKnots() const { return k; }

    // 获取 B-Spline 阶数
    int getDegree() const { return degree; }
};

#endif // BSPLINEBASIS_H

