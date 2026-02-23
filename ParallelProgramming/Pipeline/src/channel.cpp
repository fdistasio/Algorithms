#include "../include/channel.hpp"

Channel::Channel(size_t cap) : capacity(cap) {}

void Channel::push(int value) {

    std::unique_lock<std::mutex> lock(mtx);
    notFull.wait(lock, [&]() { return queue.size() < capacity; });
    queue.push_back(value);
    lock.unlock();
    notEmpty.notify_one();

}

int Channel::pop() {

    std::unique_lock<std::mutex> lock(mtx);
    notEmpty.wait(lock, [&]() { return !queue.empty(); });
    int value = queue.front();
    queue.pop_front();
    lock.unlock();
    notFull.notify_one();
    return value;

}