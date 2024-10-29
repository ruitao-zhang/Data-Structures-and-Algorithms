#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>

// 牛顿插值法的实现
double newtonInterpolation(const std::vector<double>& x, const std::vector<double>& y, double target) {
    int n = x.size();
    std::vector<std::vector<double>> dividedDiffs(n, std::vector<double>(n));

    for (int i = 0; i < n; i++) {
        dividedDiffs[i][0] = y[i];
    }

    for (int j = 1; j < n; j++) {
        for (int i = 0; i < n - j; i++) {
            dividedDiffs[i][j] = (dividedDiffs[i + 1][j - 1] - dividedDiffs[i][j - 1]) / (x[i + j] - x[i]);
        }
    }

    double result = dividedDiffs[0][0];
    double product = 1.0;
    for (int i = 1; i < n; i++) {
        product *= (target - x[i - 1]);
        result += dividedDiffs[0][i] * product;
    }
    return result;
}

// Hermite basis functions and their derivatives
double h00(double u) { return (1 + 2 * u) * (1 - u) * (1 - u); }
double h10(double u) { return u * (1 - u) * (1 - u); }
double h01(double u) { return u * u * (3 - 2 * u); }
double h11(double u) { return u * u * (u - 1); }

// Derivatives of the Hermite basis functions
double h00_prime(double u, double dt) { return (6 * u * (u - 1)) / dt; }
double h10_prime(double u, double dt) { return (3 * u * u - 4 * u + 1) / dt; }
double h01_prime(double u, double dt) { return (-6 * u * (u - 1)) / dt; }
double h11_prime(double u, double dt) { return (3 * u * u - 2 * u) / dt; }

// Hermite interpolation function for displacement
double hermiteInterpolate(const std::vector<double>& time, const std::vector<double>& displacement, const std::vector<double>& velocity, double t) {
    double result = 0.0;
    for (size_t i = 0; i < time.size() - 1; ++i) {
        double t0 = time[i], t1 = time[i + 1];
        double s0 = displacement[i], s1 = displacement[i + 1];
        double v0 = velocity[i], v1 = velocity[i + 1];

        if (t >= t0 && t <= t1) {
            double u = (t - t0) / (t1 - t0);
            result += s0 * h00(u) + v0 * (t1 - t0) * h10(u)
                    + s1 * h01(u) + v1 * (t1 - t0) * h11(u);
            break;
        }
    }
    return result;
}

// Derivative of Hermite interpolation for velocity
double hermiteInterpolateDerivative(const std::vector<double>& time, const std::vector<double>& displacement, const std::vector<double>& velocity, double t) {
    double result = 0.0;
    for (size_t i = 0; i < time.size() - 1; ++i) {
        double t0 = time[i], t1 = time[i + 1];
        double s0 = displacement[i], s1 = displacement[i + 1];
        double v0 = velocity[i], v1 = velocity[i + 1];
        double dt = t1 - t0;

        if (t >= t0 && t <= t1) {
            double u = (t - t0) / dt;
            result += s0 * h00_prime(u, dt) + v0 * h10_prime(u, dt) * dt
                    + s1 * h01_prime(u, dt) + v1 * h11_prime(u, dt) * dt;
            break;
        }
    }
    return result;
}

// 生成 Runge 现象和 Chebyshev 插值的数据
void generateData(const std::vector<int>& ns, double (*f)(double), const std::string& filename_prefix, double x_start, double x_end) {
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
            file << x << "\t" << f(x) << "\t" << newtonInterpolation(xi, fi, x) << "\n";
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

    double targetTime = 10.0;

    // Part (a): Predict displacement and velocity at t = 10
    double predictedPosition = hermite(time, displacement, velocity, targetTime);
    double predictedVelocity = hermite1(time, displacement, velocity, targetTime);

    std::cout << "position: " << predictedPosition << " feet" << std::endl;
    std::cout << "velocity: " << predictedVelocity << " feet per second" << std::endl;

    // Part (b): Check if the car ever exceeds the speed limit (81 feet per second)
    bool exceedsSpeedLimit = false;
    for (double t = time.front(); t <= time.back(); t += 0.1) {
        double velocityAtT = hermiteInterpolateDerivative(time, displacement, velocity, t);
        if (velocityAtT > 81.0) {
            exceedsSpeedLimit = true;
            break;
        }
    }

    if (exceedsSpeedLimit) {
        std::cout << "The car exceeds the speed limit of 81 feet per second." << std::endl;
    } else {
        std::cout << "The car does not exceed the speed limit of 81 feet per second." << std::endl;
    }

    // 第五个问题：幼虫体重预测
    std::vector<double> days = {0, 6, 10, 13, 17, 20, 28};
    std::vector<double> sp1 = {6.67, 17.3, 42.7, 37.3, 30.1, 29.3, 28.7};
    std::vector<double> sp2 = {6.67, 16.1, 18.9, 15.0, 10.6, 9.44, 8.89};
    double day = 43;
    double sp1 = newton(days, sp1, day)/1000;
    double sp2 = newton(days, sp2, day)/1000;
    if (sp1 <= 0) {
        std::cout << "sp1 die." << std::endl;
    } else {
        std::cout << "sp1 survive." << std::endl;
    }
    if (sp2 <= 0) {
        std::cout << "sp2 die." << std::endl;
    } else {
        std::cout << "sp2 survive." << std::endl;
    }

    // 第六个问题：近似心形曲线
    // 使用 Python 绘制
    std::ofstream bezier_file("heart_curve.txt");
    bezier_file << "x y";
    for (double theta = -3.14; theta <= 3.14; theta += 0.01) {
        double x = 2 * std::pow(sin(theta), 3);
        double y = 1.5 * cos(theta) - 0.6 * cos(2 * theta) - 0.3 * cos(3 * theta) - 0.1 * cos(4 * theta);
        bezier_file << x << " " << y << "";
    }
    bezier_file.close();

    return 0;
}
