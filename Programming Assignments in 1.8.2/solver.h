// solvers.h
#ifndef SOLVERS_H
#define SOLVERS_H

#include <iostream>
#include <cmath>
#include <functional>

// 抽象基类 EquationSolver
class EquationSolver {
public:
    virtual double solve(double initialGuess = 0) = 0;
};

// 二分法求解类
class BisectionSolver : public EquationSolver {
private:
    double a, b, tolerance;
    std::function<double(double)> func;

public:
    BisectionSolver(double a, double b, double tol, std::function<double(double)> f)
        : a(a), b(b), tolerance(tol), func(f) {}

    double solve(double initialGuess = 0) override;
};

// 牛顿法求解类
class NewtonSolver : public EquationSolver {
private:
    double tolerance;
    std::function<double(double)> func;
    std::function<double(double)> derivative;

public:
    NewtonSolver(double tol, std::function<double(double)> f, std::function<double(double)> df)
        : tolerance(tol), func(f), derivative(df) {}

    double solve(double initialGuess) override;
};

// 割线法求解类
class SecantSolver : public EquationSolver {
private:
    double tolerance;
    std::function<double(double)> func;

public:
    SecantSolver(double tol, std::function<double(double)> f)
        : tolerance(tol), func(f) {}

    double solve(double x0, double x1);
    double solve(double initialGuess = 0) override;
};

// 将角度从度转换为弧度
double degreesToRadians(double degrees);

// 将弧度转换为角度
double radiansToDegrees(double radians);

#endif 
