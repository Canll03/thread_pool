#include "threadPool.h"

void test() {
    threadPool& pool = threadPool::getInstance(10);
    for(int i = 0; i < 10; i++) {
        pool.enqueue([i]() {
            std::cout<< "task: " << i << " is running" <<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout<< "task: " << i << " is done" <<std::endl;
        });
    }
}

int main() {
    test();
    return 0;
}


