#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
#include <execution>
#include <cstddef>

using ull = unsigned long long;

bool is_prime(ull n);

size_t seq_prime(std::vector<ull>& results, ull n1, ull n2);
size_t par_prime(std::vector<ull>& results, ull n1, ull n2);