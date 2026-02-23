#include "../include/integrator.hpp"

#include <cmath>
#include <numbers>
#include <random>
#include <algorithm>

constexpr double PI = std::numbers::pi_v<double>;
constexpr double MAX_DEPTH = 30.0;

// kind: 0 = not heavy; 1 = heavy high frequency; 2 = heavy 
inline double f_eval(int kind, double param, double x) {
    if (kind == 0) return std::sin(2.0 * PI * 10.0 * x);
    if (kind == 1) return std::sin(2.0 * PI * param * x);
    // kind == 2, picco quasi singolare
    double t = std::abs(x - param);
    return 1.0 / std::sqrt(t + 1e-6);
}

// Simpson's rule on [a,b] given f(a)=fa, f((a+b)/2)=fm, f(b)=fb
inline double simpson(double fa, double fm, double fb, double a, double b) {
    return (b - a) * (fa + 4.0 * fm + fb) / 6.0;
}

// Recursive adaptive Simpson with Richardson correction
//    - eps: absolute tolerance for the sub-interval 
//    - depth: current recursion depth
double adapt_impl(int kind, double param, double a, double b,
                  double fa, double fm, double fb,
                  double S, double eps, int depth) {

    const double m  = 0.5 * (a + b);
    const double lm = 0.5 * (a + m);
    const double rm = 0.5 * (m + b);

    const double flm = f_eval(kind, param, lm);
    const double frm = f_eval(kind, param, rm);

    const double Sl = simpson(fa,  flm, fm, a, m);
    const double Sr = simpson(fm,  frm, fb, m, b);
    const double D  = std::abs((Sl + Sr) - S);

    if (D < 15.0 * eps || depth > MAX_DEPTH)
        return Sl + Sr + (Sl + Sr - S) / 15.0;

    return adapt_impl(kind, param, a, m, fa,  flm, fm, Sl, eps * 0.5, depth + 1) +
           adapt_impl(kind, param, m, b, fm,  frm, fb, Sr, eps * 0.5, depth + 1);
}

double integrate_adaptive(int kind, double param,
                          double a, double b, double eps) {

    const double m  = 0.5 * (a + b);
    const double fa = f_eval(kind, param, a);
    const double fm = f_eval(kind, param, m);
    const double fb = f_eval(kind, param, b);
    const double S  = simpson(fa, fm, fb, a, b);
    return adapt_impl(kind, param, a, b, fa, fm, fb, S, eps, 0);

}

std::vector<Job> make_dataset(std::size_t N, double heavy_ratio) {

    std::vector<Job> data(N);
    std::mt19937 rng(123);
    std::uniform_real_distribution<double> U01(0.0, 1.0);
    std::uniform_real_distribution<double> Ufreq(500.0, 1500.0);

    const std::size_t heavy = static_cast<std::size_t>(N * heavy_ratio);

    for (std::size_t i = 0; i < heavy; ++i) {
        if (i & 1) data[i] = {1, Ufreq(rng)};
        else       data[i] = {2, U01(rng)};
    }

    for (std::size_t i = heavy; i < N; ++i) {
        data[i] = {0, 0.0};
    }

    return data;

}