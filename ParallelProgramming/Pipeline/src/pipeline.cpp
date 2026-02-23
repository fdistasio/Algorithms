#include "../include/pipeline.hpp"
#include <cstdio>
#include <chrono>

#define END_TOKEN -1

void source(Channel& out, int N) {

    for (int i = 0; i <= N; ++i) {
        out.push(i);
    }
    out.push(END_TOKEN);

}

void filterA(Channel& in, Channel& out) {

    while (true) {
        int value = in.pop();

        if (value == END_TOKEN) {
            out.push(END_TOKEN);
            break;
        }

        if (value % 3 == 0) {
            out.push(value);
        }
    }

}

void filterB(Channel& in, Channel& out) {

    while (true) {
        int value = in.pop();

        if (value == END_TOKEN) {
            out.push(END_TOKEN);
            break;
        }

        out.push(value * value);
    }

}

void sink(Channel& in) {

    long long sum = 0;

    while (true) {
        int value = in.pop();

        if (value == END_TOKEN)
            break;

        sum += value;
    }

    std::printf("Sum: %lld\n", sum);

}

void sequential(int N) {

    auto t0 = std::chrono::steady_clock::now();

    long long sum = 0;
    for (int i = 0; i <= N; ++i) {
        if (i % 3 == 0)
            sum += static_cast<long long>(i) * i;
    }

    auto t1 = std::chrono::steady_clock::now();
    double time = std::chrono::duration<double, std::milli>(t1 - t0).count();

    std::printf("Sequential Sum: %lld\n", sum);
    std::printf("Sequential Time: %.3f ms\n", time);

}