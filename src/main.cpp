#include <iostream>
#include <thread>

#include "windows/api.h"
#include "hook/hook.h"

int main() {
    Hook::init();
    int exitCode = API::startLoop();
    Hook::cleanup();
    return exitCode;
}
