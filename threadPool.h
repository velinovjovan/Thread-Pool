#include<vector>
#include<queue>
#include<functional>
#include<thread>
#include<mutex>
#include<condition_variable>

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : stop(false) {
        for (size_t i{0}; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    condition.wait(lock, [this] {
                        return stop || !tasks.empty();
                    });

                    if (stop && tasks.empty()) {
                        break;
                    }

                    auto task = std::move(tasks.front());
                    tasks.pop();
                    lock.unlock();
                    task();
                }
            });
        }
    }

    template<class F>
    void enqueue(F&& task) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            tasks.emplace(std::forward<F>(task));
        }
        condition.notify_one();
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();

        for (auto& worker : workers) {
            worker.join();
        }
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};