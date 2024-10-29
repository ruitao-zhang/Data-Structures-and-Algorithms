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

// 主函数
int main() {
     // 定义目标函数
    auto f1 = [](double x) { return 1.0 / (1 + x * x); };
    auto f2 = [](double x) { return 1.0 / (1 + 25 * x * x); };

    // 生成 Runge 现象的插值数据
    std::vector<int> ns1 = {2, 4, 6, 8};
    Data(ns1, f1, "runge_n", -5, 5);

    // 生成 Chebyshev 插值的数据
    std::vector<int> ns2 = {5, 10, 15, 20};
    Data(ns2, f2, "chebyshev_n", -1, 1);

    // 第四个问题：Hermite 插值预测车辆位置和速度
    // Given data points
    std::vector<double> time = {0, 3, 5, 8, 13};
    std::vector<double> displacement = {0, 225, 383, 623, 993};
    std::vector<double> velocity = {75, 77, 80, 74, 72};

    double Time = 10.0;

    // Part (a): Predict displacement and velocity at t = 10
    double Position = hermite(time, displacement, velocity, Time);
    double Velocity = hermite1(time, displacement, velocity, Time);

    std::cout << "position: " << Position << " feet" << std::endl;
    std::cout << "velocity: " << Velocity << " feet per second" << std::endl;

    // Part (b): Check if the car ever exceeds the speed limit (81 feet per second)
    bool Limit = false;
    for (double t = time.front(); t <= time.back(); t += 0.1) {
        double speed = hermite1(time, displacement, velocity, t);
        if (speed > 81.0) {
            Limit = true;
            break;
        }
    }

    if (Limit) {
        std::cout << "Overspped" << std::endl;
    } else {
        std::cout << "No overspeed" << std::endl;
    }

    // 第五个问题：幼虫体重预测
    std::vector<double> days = {0, 6, 10, 13, 17, 20, 28};
    std::vector<double> sp1 = {6.67, 17.3, 42.7, 37.3, 30.1, 29.3, 28.7};
    std::vector<double> sp2 = {6.67, 16.1, 18.9, 15.0, 10.6, 9.44, 8.89};
    double day = 43;
    double sp_1 = newton(days, sp1, day)/1000;
    double sp_2 = newton(days, sp2, day)/1000;
    if (sp_1 <= 0) {
        std::cout << "sp1: " << sp_1 << "  die" <<std::endl;
    } else {
        std::cout << "sp1: " << sp_1 << "  survive" << std::endl;
    }
    if (sp_2 <= 0) {
        std::cout << "sp2: " << sp_2 << "  die" << std::endl;
    } else {
        std::cout << "sp2: " << sp_2 << "  survive" << std::endl;
    }
    return 0;
}
