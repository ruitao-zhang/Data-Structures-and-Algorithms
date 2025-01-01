#ifndef CUBICSPLINE_H
#define CUBICSPLINE_H

#include <vector>
#include <stdexcept>
#include <cmath>

// 定义三次样条插值类
class CubicSpline {
private:
    std::vector<double> x;      // 样本点 x 坐标
    std::vector<double> y;      // 样本点 y 坐标
    std::vector<double> a;      // 常数项
    std::vector<double> b;      // 一次项系数
    std::vector<double> c;      // 二次项系数
    std::vector<double> d;      // 三次项系数

public:
    // 构造函数：输入样本点 x 和 y
    CubicSpline(const std::vector<double>& x, const std::vector<double>& y) : x(x), y(y) {
        if (x.size() != y.size() || x.size() < 2) {
            throw std::invalid_argument("Invalid");
        }

        // 检查 x 是否单调递增
        for (size_t i = 1; i < x.size(); ++i) {
            if (x[i] <= x[i - 1]) {
                throw std::invalid_argument("Invalid.");
            }
        }

        g_0(); // 计算三次样条的系数
    }

    // 计算三次样条插值结果
    double f_3(double xval) const {
        // 找到 xval 所在的区间
        size_t n = x.size() - 1;
        size_t i = findInterval(xval);

        // 根据三次多项式计算结果
        double dx = xval - x[i];
        return a[i] + b[i] * dx + c[i] * dx * dx + d[i] * dx * dx * dx;
    }

private:
    // 计算三次样条系数
    void g_0() {
        size_t n = x.size() - 1; // 区间数
        std::vector<double> h(n); // 每个区间的宽度
        for (size_t i = 0; i < n; ++i) {
            h[i] = x[i + 1] - x[i];
        }

        // 计算矩阵的右端项 alpha
        std::vector<double> alpha(n);
        for (size_t i = 1; i < n; ++i) {
            alpha[i] = (3.0 / h[i] * (y[i + 1] - y[i]) - 3.0 / h[i - 1] * (y[i] - y[i - 1]));
        }

        // 构造三对角矩阵
        std::vector<double> l(n + 1, 0.0), mu(n, 0.0), z(n + 1, 0.0);
        l[0] = 1.0; // 边界条件
        for (size_t i = 1; i < n; ++i) {
            l[i] = 2.0 * (x[i + 1] - x[i - 1]) - h[i - 1] * mu[i - 1];
            mu[i] = h[i] / l[i];
            z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
        }
        l[n] = 1.0; // 边界条件

        // 反向回代求解 c, b, d
        c.resize(n + 1, 0.0);
        b.resize(n, 0.0);
        d.resize(n, 0.0);
        a.resize(n, 0.0);

        for (int j = n - 1; j >= 0; --j) {
            c[j] = z[j] - mu[j] * c[j + 1];
            b[j] = (y[j + 1] - y[j]) / h[j] - h[j] * (c[j + 1] + 2.0 * c[j]) / 3.0;
            d[j] = (c[j + 1] - c[j]) / (3.0 * h[j]);
            a[j] = y[j];
        }
    }

    // 找到 xval 所属的区间索引
    size_t g_2(double xval) const {
        if (xVal < x.front() || xVal > x.back()) {
            throw std::out_of_range("Out");
        }

        size_t low = 0, high = x.size() - 1;
        while (low < high - 1) {
            size_t mid = (low + high) / 2;
            if (xval < x[mid]) {
                low = mid;
            } else {
                high = mid;
            }
        }
        return low;
    }
};

#endif // CUBICSPLINE_H

