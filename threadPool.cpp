#include "threadPool.h"

std::unique_ptr<threadPool> threadPool::instance = nullptr;
std::once_flag threadPool::initFlag;

threadPool& threadPool::getInstance(int threadNum) {
    std::call_once(initFlag, [&]() {
        instance.reset(new threadPool(threadNum));
    });
    return *instance;
}

threadPool::~threadPool() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }

    condition.notify_all();
    for(auto &t : _threads) {
        t.join();
    }
}

// template<class F, class... Args>
// void threadPool::enqueue(F&& f, Args&&... args) {
//     auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
//     {
//         std::unique_lock<std::mutex> lock(mtx);
//         tasks.emplace(std::move(task));
//     }
//     condition.notify_all();
// }

threadPool::threadPool(int threadNum) : stop(false) {
    for(int i = 0; i < threadNum; i++) {
        _threads.emplace_back([this] {
            while(1) {
                std::unique_lock<std::mutex> umtx(mtx);
                condition.wait(umtx, [this]() {
                    return !tasks.empty() || stop;
                });

                if(stop && tasks.empty()) {
                    return;
                }

                std::function<void()> task(std::move(tasks.front()));
                tasks.pop();

                umtx.unlock();
                task();
            }
        });
    }
}
