#pragma once
#include <deque>
#include <mutex>
#include <condition_variable>

class Channel {
public:
    Channel(size_t capacity);

    void push(int value);
    int pop();

private:
    std::deque<int> queue;
    std::mutex mtx;
    std::condition_variable notEmpty;
    std::condition_variable notFull;
    size_t capacity;
};