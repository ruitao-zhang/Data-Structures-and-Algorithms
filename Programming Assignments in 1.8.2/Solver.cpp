#include <iostream>
#include <cmath>
#include <functional>
#include <tuple>  

// 抽象基类 EquationSolver
class EquationSolver {
public:
    virtual double solve(double initialGuess = 0) = 0;
};

// 二分法
class BisectionSolver : public EquationSolver {
private:
    double a, b, tolerance;
    std::function<double(double)> func;

public:
    BisectionSolver(double a, double b, double tol, std::function<double(double)> f)
        : a(a), b(b), tolerance(tol), func(f) {}

    double solve(double initialGuess = 0) override {
        double mid;
        while ((b - a) / 2 > tolerance) {
            mid = (a + b) / 2;
            if (func(mid) == 0.0)
                return mid;
            else if (func(a) * func(mid) < 0)
                b = mid;
            else
                a = mid;
        }
        return (a + b) / 2;
    }
};

// 牛顿法
class NewtonSolver : public EquationSolver {
private:
    double tolerance;
    std::function<double(double)> func;
    std::function<double(double)> derivative;

public:
    NewtonSolver(double tol, std::function<double(double)> f, std::function<double(double)> df)
        : tolerance(tol), func(f), derivative(df) {}

    double solve(double initialGuess) override {
        double x = initialGuess;
        double h = func(x) / derivative(x);
        while (std::fabs(h) > tolerance) {
            h = func(x) / derivative(x);
            x = x - h;
        }
        return x;
    }
};

// 割线法
class SecantSolver : public EquationSolver {
private:
    double tolerance;
    std::function<double(double)> func;

public:
    SecantSolver(double tol, std::function<double(double)> f)
        : tolerance(tol), func(f) {}

    double solve(double x0, double x1) {
        double x2;
        while (std::fabs(x1 - x0) > tolerance) {
            x2 = x1 - func(x1) * (x1 - x0) / (func(x1) - func(x0));
            x0 = x1;
            x1 = x2;
        }
        return x1;
    }

    double solve(double initialGuess = 0) override {
        return solve(initialGuess, initialGuess + 1.0);
    }
};

// 将角度从度转换为弧度
double degreesToRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

// 将弧度转换为角度
double radiansToDegrees(double radians) {
    return radians * 180.0 / M_PI;
}

int main() {
    // 2. 使用二分法测试特定函数
    std::cout << "=== 问题2: 测试二分法 ===";

    auto func1 = [](double x) { return 1.0 / x - std::tan(x); };
    BisectionSolver bisect1(0.1, M_PI / 2 - 0.1, 0.0001, func1);
    std::cout << "Root of 1/x - tan(x) in [0, pi/2]: " << bisect1.solve() << std::endl;

    auto func2 = [](double x) { return 1.0 / x - std::pow(2, x); };
    BisectionSolver bisect2(0.1, 1.0, 0.0001, func2);
    std::cout << "Root of 1/x - 2^x in [0, 1]: " << bisect2.solve() << std::endl;

    auto func3 = [](double x) { return std::pow(2, -x) + std::exp(x) + 2 * std::cos(x) - 6; };
    BisectionSolver bisect3(1.0, 3.0, 0.0001, func3);
    std::cout << "Root of 2^(-x) + e^x + 2cos(x) - 6 in [1, 3]: " << bisect3.solve() << std::endl;

    auto func4 = [](double x) { return (x * x * x + 4 * x * x + 3 * x + 5) / (2 * x * x * x - 9 * x * x + 18 * x - 2); };
    BisectionSolver bisect4(0.1, 4.0, 0.0001, func4);
    std::cout << "Root of rational function in [0, 4]: " << bisect4.solve() << std::endl;

    // 3. 牛顿法求解 x = tan(x)
    std::cout << "=== 问题3: 牛顿法求解 x = tan(x) ===";

    auto func_newton_tan = [](double x) { return x - std::tan(x); };
    auto derivative_newton_tan = [](double x) { return 1 - std::pow(1.0 / std::cos(x), 2); };

    NewtonSolver newton1(0.0001, func_newton_tan, derivative_newton_tan);
    std::cout << "Newton's Method root near 4.5: " << newton1.solve(4.5) << std::endl;

    NewtonSolver newton2(0.0001, func_newton_tan, derivative_newton_tan);
    std::cout << "Newton's Method root near 7.7: " << newton2.solve(7.7) << std::endl;

    // 4. 使用割线法测试
    std::cout << "=== 问题4: 测试割线法 ===";

        auto func_secant1 = [](double x) { return std::sin(x / 2) - 1; };
    SecantSolver secant1(0.0001, func_secant1);

    // 原初值 0 和 pi/2
    std::cout << "Secant Method root of sin(x/2) - 1 with initial values 0 and pi/2: "
              << secant1.solve(0, M_PI / 2) << std::endl;

    // 新初值 0.1 和 1.5
    std::cout << "Secant Method root of sin(x/2) - 1 with new initial values 0.1 and 1.5: "
              << secant1.solve(0.1, 1.5) << std::endl;

    auto func_secant2 = [](double x) { return std::exp(x) - std::tan(x); };
    SecantSolver secant2(0.0001, func_secant2);

    // 原初值 1 和 1.4
    std::cout << "Secant Method root of e^x - tan(x) with initial values 1 and 1.4: "
              << secant2.solve(1, 1.4) << std::endl;

    // 新初值 0.5 和 1.3
    std::cout << "Secant Method root of e^x - tan(x) with new initial values 0.5 and 1.3: "
              << secant2.solve(0.5, 1.3) << std::endl;

    auto func_secant3 = [](double x) { return x * x * x - 12 * x * x + 3 * x + 1; };
    SecantSolver secant3(0.0001, func_secant3);

    // 原初值 0 和 -0.5
    std::cout << "Secant Method root of x^3 - 12x^2 + 3x + 1 with initial values 0 and -0.5: "
              << secant3.solve(0, -0.5) << std::endl;

    // 新初值 1 和 2
    std::cout << "Secant Method root of x^3 - 12x^2 + 3x + 1 with new initial values 1 and 2: "
              << secant3.solve(1, 2) << std::endl;

    // 5. 体积问题
    std::cout << "=== 问题5: 体积问题 ===";

    double L = 10.0;
    double r = 1.0;
    double V = 12.4;

    auto volumeFunc = [L, r, V](double h) {
        return L * (0.5 * M_PI * r * r - r * r * std::asin(h / r) - h * std::sqrt(r * r - h * h)) - V;
    };
    auto volumeFuncDerivative = [L, r](double h) {
        return L * (-r * r / std::sqrt(r * r - h * h) - std::sqrt(r * r - h * h));
    };

    BisectionSolver bisectVolume(0.0, r, 0.01, volumeFunc);
    std::cout << "Bisection Method (Water depth h): " << bisectVolume.solve() << std::endl;

    NewtonSolver newtonVolume(0.01, volumeFunc, volumeFuncDerivative);
    std::cout << "Newton's Method (Water depth h): " << newtonVolume.solve(0.5) << std::endl;

    SecantSolver secantVolume(0.01, volumeFunc);
    std::cout << "Secant Method (Water depth h): " << secantVolume.solve(0.1, 0.9) << std::endl;
 
// 6. 全地形车失效角度问题
    std::cout << "=== 问题6: 全地形车失效角度问题 ===";

    double l = 89.0;
    double h = 49.0;
    double D1 = 55.0;
    double D2 = 30.0;
    double beta1_deg = 11.5;
    double beta1 = degreesToRadians(beta1_deg);

    auto calculateCoefficients = [l, h, beta1](double D) {
        double A = l * std::sin(beta1);
        double B = l * std::cos(beta1);
        double C = (h + 0.5 * D) * std::sin(beta1) - 0.5 * D * std::tan(beta1);
        double E = (h + 0.5 * D) * std::cos(beta1) - 0.5 * D;
        return std::make_tuple(A, B, C, E);
    };

    auto createFunctions = [](double A, double B, double C, double E) {
        auto func = [A, B, C, E](double alpha) {
            return A * std::sin(alpha) * std::cos(alpha) + B * std::pow(std::sin(alpha), 2)
                   - C * std::cos(alpha) - E * std::sin(alpha);
        };
        auto derivative = [A, B, C, E](double alpha) {
            return A * (std::pow(std::cos(alpha), 2) - std::pow(std::sin(alpha), 2))
                   + 2 * B * std::sin(alpha) * std::cos(alpha) + C * std::sin(alpha)
                   - E * std::cos(alpha);
        };
        return std::make_tuple(func, derivative);
    };

    std::tuple<double, double, double, double> coefficients1 = calculateCoefficients(D1);
    double A1 = std::get<0>(coefficients1);
    double B1 = std::get<1>(coefficients1);
    double C1 = std::get<2>(coefficients1);
    double E1 = std::get<3>(coefficients1);

    std::tuple<std::function<double(double)>, std::function<double(double)>> functions1 = createFunctions(A1, B1, C1, E1);
    std::function<double(double)> func1_car = std::get<0>(functions1);
    std::function<double(double)> derivative1_car = std::get<1>(functions1);

    NewtonSolver newtonSolver1(0.0001, func1_car, derivative1_car);
    double alpha1_rad = newtonSolver1.solve(degreesToRadians(33));
    std::cout << "Verified alpha for D = 55 inches: " << radiansToDegrees(alpha1_rad) << " degrees" << std::endl;

    std::tuple<double, double, double, double> coefficients2 = calculateCoefficients(D2);
    double A2 = std::get<0>(coefficients2);
    double B2 = std::get<1>(coefficients2);
    double C2 = std::get<2>(coefficients2);
    double E2 = std::get<3>(coefficients2);

    std::tuple<std::function<double(double)>, std::function<double(double)>> functions2 = createFunctions(A2, B2, C2, E2);
    std::function<double(double)> func2_car = std::get<0>(functions2);
    std::function<double(double)> derivative2_car = std::get<1>(functions2);

    NewtonSolver newtonSolver2(0.0001, func2_car, derivative2_car);
    double alpha2_rad = newtonSolver2.solve(degreesToRadians(33));
    std::cout << "Alpha for D = 30 inches using Newton's method: " << radiansToDegrees(alpha2_rad) << " degrees" << std::endl;

    SecantSolver secantSolver(0.0001, func2_car);
    double alpha_secant_rad = secantSolver.solve(degreesToRadians(20), degreesToRadians(60));
    std::cout << "Alpha using Secant method: " << radiansToDegrees(alpha_secant_rad) << " degrees" << std::endl;

    return 0;
    return 0;
}
