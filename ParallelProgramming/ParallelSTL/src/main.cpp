/*
This program computes all prime numbers in a given range [n1, n2].
Each prime is tested independently, and the computation is done in two ways:
- Sequentially using seq_prime
- Parallel using par_prime with STL parallel algorithms

Optionally, all found primes can be printed by passing "on" as a third argument.

Usage:
./primes n1 n2 [print=off|on]
*/

#include <iostream>
#include <chrono>
#include "../include/primes.hpp"

using clock_type = std::chrono::steady_clock;

int main(int argc, char *argv[]) {

    if (argc < 3) {
        std::cerr << "use: " << argv[0] << " number1 number2 [print=off|on]\n";
        return -1;
    }

    ull n1 = std::stoull(argv[1]);
    ull n2 = std::stoull(argv[2]);
    bool printPrimes = (argc == 4 && std::string(argv[3]) == "on");

    std::vector<ull> resultsSeq, resultsPar;

    auto t0 = clock_type::now();
    size_t countSeq = seq_prime(resultsSeq, n1, n2);
    auto t1 = clock_type::now();

    std::cout << "Sequential: " << countSeq << " primes in "
              << std::chrono::duration<double>(t1 - t0).count() << " s\n";

    t0 = clock_type::now();
    size_t countPar = par_prime(resultsPar, n1, n2);
    t1 = clock_type::now();

    std::cout << "Parallel: " << countPar << " primes in "
              << std::chrono::duration<double>(t1 - t0).count() << " s\n";

    if (printPrimes) {
        for(auto r: resultsSeq) std::cout << r << " ";
        std::cout << "\n";
        for(auto r: resultsPar) std::cout << r << " ";
        std::cout << "\n";
    }

    return 0;
}