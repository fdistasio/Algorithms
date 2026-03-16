#include "../include/primes.hpp"

// Prime Test
bool is_prime(ull n) {

    if (n <= 3) return n > 1;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (ull i = 5; i <= n/i; i += 6)
        if (n % i == 0 || n % (i+2) == 0) return false;
    return true;

}

// Sequential
size_t seq_prime(std::vector<ull>& results, ull n1, ull n2) {

    for (ull i = n1; i < n2; ++i)
        if (is_prime(i))
            results.push_back(i);
    return results.size();

}

// Parallel STL
size_t par_prime(std::vector<ull>& results, ull n1, ull n2) {

    std::vector<ull> range(n2 - n1);
    std::iota(range.begin(), range.end(), n1);

    results.resize(range.size());
    auto it = std::copy_if(std::execution::par, range.begin(), range.end(), results.begin(), is_prime);
    results.resize(it - results.begin());

    return results.size();

}