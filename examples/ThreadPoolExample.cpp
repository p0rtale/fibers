#include <iostream>

#include <fibers/thread_pool/ThreadPool.hpp>

using namespace fibers;

int main() {
    ThreadPool pool(3);

    pool.addTask([]() {
        std::cout << "H";
        currentThreadPool()->addTask([]() {
            std::cout << "e";
            currentThreadPool()->addTask([]() {
                std::cout << "l";
            });
        });
    });

    pool.addTask([]() {
        std::cout << "l";
        currentThreadPool()->addTask([]() {
            std::cout << "o";
            currentThreadPool()->addTask([]() {
                std::cout << "!";
            });
        });
    });

    pool.wait();
    pool.stop();

    std::cout << "\n";

    return EXIT_SUCCESS;
}
