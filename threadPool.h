#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>
#include <memory>

class threadPool {
public:
    static threadPool& getInstance(int threadNum);

    threadPool(const threadPool&) = delete;
    threadPool& operator=(const threadPool&) = delete;

    ~threadPool();

    template<class F, class... Args>
    void enqueue(F&& f, Args&&... args) {
        auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        {
            std::unique_lock<std::mutex> lock(mtx);
            tasks.emplace(std::move(task));
        }
        condition.notify_all();
    }

private:
    threadPool(int threadNum);

    std::vector<std::thread> _threads;
    std::queue<std::function<void()>> tasks;

    std::mutex mtx;
    std::condition_variable condition;
    bool stop;

    static std::unique_ptr<threadPool> instance;
    static std::once_flag initFlag;
};

//std::unique_ptr<threadPool> instance = nullptr;
//std::once_flag initFlag;

//#include "threadPool.cpp"

#endif // THREADPOOL_H
