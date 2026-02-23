/*
This program computes N independent definite integrals on [0,1] using an
adaptive Simpson method with Richardson correction.
Each integral uses one of three integrands: an easy sinusoid,
a high-frequency sinusoid, or a near-singular function.
There is a fraction of tasks (heavy_ratio) that needs several iterations,
the rest are not so heavy.

Three different implementations are provided:
- Sequential
- Parallel with task block cyclic assignment
- Parallel with task assignment via work sharing
*/

#include "../include/integrator.hpp"
#include "../include/strategies.hpp"

#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>
#include <string>

using clock_type = std::chrono::steady_clock;


bool parse_params(int argc, char* argv[], std::size_t& N, double& heavy_ratio, int& execType) {

    N = 1000;
    heavy_ratio = 0.2;
    execType = 0;

    if (argc >= 2) {
        N = std::stoull(argv[1]);
        if (N == 0) {
            std::cerr << "Error: N must be positive.\n";
            return false;
        }
    }

    if (argc >= 3) {
        heavy_ratio = std::stod(argv[2]);
        if (heavy_ratio < 0.0 || heavy_ratio >= 1.0) {
            std::cerr << "Error: heavy_ratio must be in [0,1).\n";
            return false;
        }
    }

    if (argc >= 4) {
        execType = std::stoi(argv[3]);
        if (execType < 0 || execType > 2) {
            std::cerr << "Error: execType must be 0, 1, or 2.\n";
            return false;
        }
    }

    return true;
}

int main(int argc, char* argv[]) {

    std::size_t N;
    double heavy_ratio;
    int execType;

    if (!parse_params(argc, argv, N, heavy_ratio, execType)) {
        std::cerr << "Usage: " << argv[0] << " [N] [heavy_ratio] [execType]\n";
        return -1;
    }

    auto data = make_dataset(N, heavy_ratio);
    std::vector<double> results(N);

    auto t0 = clock_type::now();

    switch (execType) {
        case 0:
            for (std::size_t i = 0; i < N; ++i)
                results[i] = integrate_adaptive(data[i].kind, data[i].param);
            break;
        case 1:
            block_cyclic_integrate(data, results);
            break;
        case 2:
            work_sharing_integrate(data, results);
            break;
    }

    auto t1 = clock_type::now();
    double tsec = std::chrono::duration<double>(t1 - t0).count();
    double checksum = std::accumulate(results.begin(), results.end(), 0.0);

    std::cout << "N = " << N
              << "   heavy_ratio = " << heavy_ratio*100 << "%\n";
    std::cout.setf(std::ios::fixed); std::cout.precision(6);
    std::cout << "time = " << tsec << " (s)\n";
    std::cout << "checksum = " << checksum << "\n";

    return 0;

}