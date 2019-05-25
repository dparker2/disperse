#include "worker.h"
#include "../windows/api.h"

#include <queue>
#include <mutex>

std::mutex mutex;
std::queue<Tailor::command> queue;

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
    }
}

Worker::Worker() {
    this->thread_obj = std::thread(workerMain);
}

void Worker::process(Tailor::command data) {
    mutex.lock();
    queue.push(data);
    mutex.unlock();
}
