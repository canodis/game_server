// #include "ThreadPool.hpp"

// ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
//     for (size_t i = 0; i < numThreads; ++i) {
//         workers.emplace_back(std::bind(&ThreadPool::workerThread, this));
//     }
// }

// ThreadPool::~ThreadPool() {
//     {
//         std::unique_lock<std::mutex> lock(tasksMutex);
//         stop = true;
//     }
//     condition.notify_all();
//     for (std::thread& worker : workers) {
//         worker.join();
//     }
// }

// void ThreadPool::workerThread() {
//     while (true) {
//         std::function<void()> task;
//         {
//             std::unique_lock<std::mutex> lock(tasksMutex);
//             condition.wait(lock, [this] { return stop || !tasks.empty(); });
//             if (stop && tasks.empty()) {
//                 return;
//             }
//             task = std::move(tasks.front());
//             tasks.pop();
//         }
//         task();
//     }
// }

// template<class F, class... Args>
// auto ThreadPool::enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
//     using ReturnType = typename std::result_of<F(Args...)>::type;
//     auto task = std::make_shared<std::packaged_task<ReturnType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
//     std::future<ReturnType> res = task->get_future();
//     {
//         std::unique_lock<std::mutex> lock(tasksMutex);
//         if (stop) {
//             throw std::runtime_error("enqueue on stopped ThreadPool");
//         }
//         tasks.emplace([task] { (*task)(); });
//     }
//     condition.notify_one();
//     return res;
// }

