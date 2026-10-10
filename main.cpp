#include <iostream>
#include <chrono>
#include "threadPool.h"

std::string get_thread_id(){
    auto myid = std::this_thread::get_id();
    std::stringstream ss;
    ss << myid;
    std::string mystr = ss.str();
    return mystr;
}


int main(){
    ThreadPool pool(std::thread::hardware_concurrency() == 0 ? 4 : std::thread::hardware_concurrency());

    std::cout << "Thread Pool Created" << std::endl;
    std::cout << "Enqueue some tasks..." << std::endl;

    for(size_t i {1} ; i <= 100 ; ++i){
        pool.enqueue([i]{
            printf("Task %zu is being processed by thread %s\n", i, get_thread_id().c_str());
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
    }

    return 0;

}