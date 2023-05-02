// #pragma once

// #include <vector>
// #include <queue>
// #include <thread>
// #include <mutex>
// #include <condition_variable>
// #include <functional>
// #include <future>
// #include <type_traits>

// class ThreadPool {
// public:
//     ThreadPool(size_t numThreads);
//     ~ThreadPool();

//     template<class F, class... Args>
//     auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

// private:
//     std::vector<std::thread> workers;
//     std::queue<std::function<void()>> tasks;
//     std::mutex tasksMutex;
//     std::condition_variable condition;
//     bool stop;

//     void workerThread();
// };

