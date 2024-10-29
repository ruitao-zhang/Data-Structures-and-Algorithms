// solvers.h
#ifndef SOLVERS_H
#define SOLVERS_H

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>

// 牛顿插值法的实现
double newton(const std::vector<double>& x, const std::vector<double>& y, double target) {
    int n = x.size();
    std::vector<std::vector<double>> Diff(n, std::vector<double>(n));

    for (int i = 0; i < n; i++) {
        Diff[i][0] = y[i];
    }

    for (int j = 1; j < n; j++) {
        for (int i = 0; i < n - j; i++) {
            Diff[i][j] = (Diff[i + 1][j - 1] - Diff[i][j - 1]) / (x[i + j] - x[i]);
        }
    }

    double result = Diff[0][0];
    double product = 1.0;
    for (int i = 1; i < n; i++) {
        product *= (target - x[i - 1]);
        result += Diff[0][i] * product;
    }
    return result;
}

// Hermite basis functions and their derivatives
double h1(double u) { return (1 + 2 * u) * (1 - u) * (1 - u); }
double h2(double u) { return u * (1 - u) * (1 - u); }
double h3(double u) { return u * u * (3 - 2 * u); }
double h4(double u) { return u * u * (u - 1); }

// Derivatives of the Hermite basis functions
double h11(double u, double dt) { return (6 * u * (u - 1)) / dt; }
double h21(double u, double dt) { return (3 * u * u - 4 * u + 1) / dt; }
double h31(double u, double dt) { return (-6 * u * (u - 1)) / dt; }
double h41(double u, double dt) { return (3 * u * u - 2 * u) / dt; }

// Hermite interpolation function for displacement
double hermite(const std::vector<double>& time, const std::vector<double>& displacement, const std::vector<double>& velocity, double t) {
    double result = 0.0;
    for (size_t i = 0; i < time.size() - 1; ++i) {
        double t0 = time[i], t1 = time[i + 1];
        double s0 = displacement[i], s1 = displacement[i + 1];
        double v0 = velocity[i], v1 = velocity[i + 1];

        if (t >= t0 && t <= t1) {
            double u = (t - t0) / (t1 - t0);
            result += s0 * h1(u) + v0 * (t1 - t0) * h2(u)
                    + s1 * h3(u) + v1 * (t1 - t0) * h4(u);
            break;
        }
    }
    return result;
}

// Derivative of Hermite interpolation for velocity
double hermite1(const std::vector<double>& time, const std::vector<double>& displacement, const std::vector<double>& velocity, double t) {
    double result = 0.0;
    for (size_t i = 0; i < time.size() - 1; ++i) {
        double t0 = time[i], t1 = time[i + 1];
        double s0 = displacement[i], s1 = displacement[i + 1];
        double v0 = velocity[i], v1 = velocity[i + 1];
        double dt = t1 - t0;

        if (t >= t0 && t <= t1) {
            double u = (t - t0) / dt;
            result += s0 * h11(u, dt) + v0 * h21(u, dt) * dt
                    + s1 * h31(u, dt) + v1 * h41(u, dt) * dt;
            break;
        }
    }
    return result;
}

// 生成 Runge 现象和 Chebyshev 插值的数据
void Data(const std::vector<int>& ns, double (*f)(double), const std::string& filename_prefix, double x_start, double x_end) {
    for (int n : ns) {
        std::ofstream file(filename_prefix + std::to_string(n) + ".txt");
        file << std::fixed << std::setprecision(5); // 固定小数位数
        file << "x\tf(x)\tp_n(x)" << "\n";

        for (double x = x_start; x <= x_end; x += 0.1) {
            std::vector<double> xi, fi;
            for (int i = 0; i <= n; i++) {
                double xi_val = x_start + i * (x_end - x_start) / n;
                xi.push_back(xi_val);
                fi.push_back(f(xi_val));
            }
            file << x << "\t" << f(x) << "\t" << newton(xi, fi, x) << "\n";
        }
        file.close();
    }
}
