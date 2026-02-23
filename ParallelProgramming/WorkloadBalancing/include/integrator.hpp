#pragma once
#include <vector>
#include <cstddef>

// A generic job: integrand family and its parameter
struct Job {
    int kind;
    double param;
};

// Create the dataset with a heavy_ratio fraction of heavy tasks
std::vector<Job> make_dataset(std::size_t N, double heavy_ratio);

// Wrapper function
double integrate_adaptive(int kind, double param,
                          double a = 0.0, double b = 1.0,
                          double eps = 1e-8);