#include "../include/strategies.hpp"
#include <thread>
#include <algorithm>
#include <future>

void block_cyclic_integrate(std::vector<Job>& data,
                            std::vector<double>& results,
                            int chunkSize, int nThreads) {

    std::vector<std::thread> workers;

    auto block_cyclic = [&](int threadId) {
        const int offset = threadId * chunkSize;
        const int stride = nThreads * chunkSize;
        for (int lower = offset; lower < data.size(); lower += stride) {
            const int upper = std::min(lower + chunkSize, (int)data.size());
            for (int i = lower; i < upper; ++i) {
                results[i] = integrate_adaptive(data[i].kind, data[i].param);
            }
        }
    };

    for (int id = 0; id < nThreads; ++id)
        workers.emplace_back([&, id]{ block_cyclic(id); });

    for (auto& t : workers) t.join();
}

void work_sharing_integrate(std::vector<Job>& data,
                            std::vector<double>& results,
                            int nThreads) {

    threadPool pool(nThreads);
    std::vector<std::future<double>> resFutures;

    auto work_sharing = [&](int index) {
        return integrate_adaptive(data[index].kind, data[index].param);
    };

    for (int i = 0; i < data.size(); ++i)
        resFutures.emplace_back(pool.submit(work_sharing, i));

    for (size_t i = 0; i < resFutures.size(); ++i)
        results[i] = resFutures[i].get();

}