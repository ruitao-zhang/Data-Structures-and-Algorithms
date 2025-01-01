#ifndef SPLINECURVEFITTING_H
#define SPLINECURVEFITTING_H

#include "CubicSpline.h" 
#include <vector>
#include <cmath>
#include <string>

class SplineCurveFitting {
private:
    std::vector<double> x, y; // 原始点
    std::vector<double> t;    // 参数化后的 t 

public:
    // 构造函数：初始化平面点
    SCF(const std::vector<double>& x, const std::vector<double>& y)
        : x(x), y(y) {
        // 弧长参数化
        t = chordLengthParameterization(x, y);
    }

    // 进行拟合
    void g_4(const std::string& filename) {
        //三次样条拟合
        CubicSpline splineX(t, x);
        CubicSpline splineY(t, y);

        // 打开输出文件
        FILE* file = fopen(filename.c_str(), "w");

        // 遍历 t 值，计算拟合曲线的点
        for (double tVal = t.front(); tVal <= t.back(); tVal += 0.01) {
            double xVal = splineX.evaluate(tVal); // 计算拟合的 x 值
            double yVal = splineY.evaluate(tVal); // 计算拟合的 y 值
            fprintf(file, "%f %f", xVal, yVal);
        }

        fclose(file);
    }

private:
    //根据点间距离生成 t 值
    std::vector<double> chordLengthParameterization(const std::vector<double>& x, const std::vector<double>& y) {
        std::vector<double> t(x.size());
        t[0] = 0.0; // 第一个点的参数为 0
        for (size_t i = 1; i < x.size(); ++i) {
            // 两点间距离作为 t 的增量
            t[i] = t[i - 1] + std::sqrt(std::pow(x[i] - x[i - 1], 2) + std::pow(y[i] - y[i - 1], 2));
        }
        return t; 
    }
};

#endif // SPLINECURVEFITTING_H

