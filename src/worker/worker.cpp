#include "worker.h"
#include "../windows/api.h"

#include <queue>
#include <mutex>
#include <atomic>

std::mutex mutex;
std::queue<Tailor::command> queue;
std::atomic<bool> quit(false);

void workerMain() {
    while (true) {
        mutex.lock();
        if (!queue.empty()) {
            Tailor::command command = queue.front();
            queue.pop();
            Tailor::alter(command);
        }
        mutex.unlock();
        API::sleep(50);
        if(quit) {
            break;
        }
    }
}

Worker::Worker() {
    this->thread_obj = std::thread(workerMain);
}

Worker::~Worker() {
    quit = true;
    this->thread_obj.join();
}

void Worker::process(Tailor::command data) {
    mutex.lock();
    queue.push(data);
    mutex.unlock();
}
