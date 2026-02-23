/*
4-stage pipeline corresponding to 4 threads:
    -Source: generates the integers 1..N and sends them to Filter A
    -Filter A: passes only multiples of 3
    -Filter B: transforms x into x*x
    -Sink: adds the received values ​​and prints the final sum
*/

#include "../include/channel.hpp"
#include "../include/pipeline.hpp"

#include <thread>
#include <vector>
#include <string>
#include <iostream>
#include <chrono>

void parse_args(int argc, char** argv, int& N, int& queueCapacity) {

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-n" && i + 1 < argc) {
            N = std::stoi(argv[++i]);
        }
        else if (arg == "-q" && i + 1 < argc) {
            queueCapacity = std::stoi(argv[++i]);
        }
    }

}

int main(int argc, char** argv) {

    int N = 10000;
    int queueCapacity = 10;
    std::vector<std::thread> pipeline;

    parse_args(argc, argv, N, queueCapacity);

    Channel ch1(queueCapacity);
    Channel ch2(queueCapacity);
    Channel ch3(queueCapacity);

    auto t0 = std::chrono::steady_clock::now();

    pipeline.emplace_back(source, std::ref(ch1), N);
    pipeline.emplace_back(filterA, std::ref(ch1), std::ref(ch2));
    pipeline.emplace_back(filterB, std::ref(ch2), std::ref(ch3));
    pipeline.emplace_back(sink, std::ref(ch3));

    for (auto& t : pipeline) {
        t.join();
    }

    auto t1_end = std::chrono::steady_clock::now();

    double parallel_time = std::chrono::duration<double, std::milli>(t1_end - t0).count();

    std::printf("Parallel Time: %.3f ms\n", parallel_time);

    sequential(N);

    return 0;

}