#ifndef WORKER_H
#define WORKER_H

#include "../tailor/tailor.h"

#include <iostream>
#include <thread>
#include <queue>
#include <mutex>

class Worker {
public:
    Worker();

    void process(Tailor::command);

private:
    std::thread thread_obj;
};

#endif
