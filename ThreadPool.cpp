#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t num_threads) : stop(false) {
	for (size_t i = 0; i < num_threads; ++i) {
		workers.emplace_back(&ThreadPool::worker_loop, this);
	}
}

ThreadPool::~ThreadPool() {
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop = true;
	}

	condition.notify_all();

	for (std::thread &worker : workers) {
		worker.join();
	}
}

void ThreadPool::enqueue(std::function<void()> task) {
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		tasks.push(task);
	}

	condition.notify_one();
}

void ThreadPool::worker_loop() {
	while (true) {
		std::function<void()> task;

		{
			std::unique_lock<std::mutex> lock(queue_mutex);

			condition.wait(lock, [this] { return !tasks.empty() || stop; });

			if (stop && tasks.empty()) {
				return;
			}

			task = std::move(tasks.front());
			tasks.pop();
		}

		task();
	}
}