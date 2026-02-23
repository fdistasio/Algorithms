#pragma once
#include "integrator.hpp"
#include "threadPool.hpp"
#include <vector>
#include <cstddef>

void block_cyclic_integrate(std::vector<Job>& data,
                            std::vector<double>& results,
                            int chunkSize = 2,
                            int nThreads = std::thread::hardware_concurrency());

void work_sharing_integrate(std::vector<Job>& data,
                            std::vector<double>& results,
                            int nThreads = std::thread::hardware_concurrency());