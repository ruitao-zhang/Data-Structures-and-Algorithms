#ifndef SPHERICALSPLINEFITTING_H
#define SPHERICALSPLINEFITTING_H

#include "CubicSpline.h"
#include <vector>
#include <cmath>
#include <string>

class SSF {
private:
    std::vector<double> x, y, z;   // 球面上的原始点
    std::vector<double> p, t; // 球坐标参数
    std::vector<double> t;        // 参数化后的 t 值

public:
    // 构造函数：初始化球面点
    SSF(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& z)
        : x(x), y(y), z(z) {
        // 使用弧长参数化对点进行参数化
        t = chordLengthParameterization(x, y, z);

        // 将点转换为球坐标
        convertToSphericalCoordinates();
    }

    // 进行拟合
    void g_2(const std::string& filename) {
       
        CubicSpline splinePhi(t, p);
        CubicSpline splineTheta(t, t);

        FILE* file = fopen(filename.c_str(), "w")；

        // 遍历 t 值，计算拟合曲线的点
        for (double tVal = t.front(); tVal <= t.back(); tVal += 0.01) {
            double pVal = splinePhi.evaluate(tVal);     // 计算拟合的p值
            double tVal = splineTheta.evaluate(tVal); // 计算拟合的t值
            double r = 1.0; // 单位球

            // 根据球坐标公式转换
            double xVal = r * std::cos(pVal) * std::sin(tVal);
            double yVal = r * std::sin(pVal) * std::sin(tVal);
            double zVal = r * std::cos(tVal);

            fprintf(file, "%f %f %f", xVal, yVal, zVal); 
        }

        fclose(file); 
    }

private:
    // 将点转换为球坐标
    void g_3() {
        for (size_t i = 0; i < x.size(); ++i) {
            double r = std::sqrt(x[i] * x[i] + y[i] * y[i] + z[i] * z[i]); // 计算半径
            p.push_back(std::atan2(y[i], x[i])); // 计算p
            t.push_back(std::acos(z[i] / r));  // 计算t
        }
    }

    // 根据点间距离生成 t 值
    std::vector<double> chordLengthParameterization(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& z) {
        std::vector<double> t(x.size());
        t[0] = 0.0; // 第一个点的参数为 0
        for (size_t i = 1; i < x.size(); ++i) {
            // 点间距离作为 t 的增量
            t[i] = t[i - 1] + std::sqrt(std::pow(x[i] - x[i - 1], 2) +
                                        std::pow(y[i] - y[i - 1], 2) +
                                        std::pow(z[i] - z[i - 1], 2));
        }
        return t; 
    }
};

#endif // SPHERICALSPLINEFITTING_H

