#ifndef WORKER_H
#define WORKER_H

#include "../tailor/tailor.h"

#include <thread>

class Worker {
public:
    Worker();
    ~Worker();
    void process(Tailor::command);

private:
    std::thread thread_obj;
};

#endif
