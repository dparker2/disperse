#ifndef HOOK_H
#define HOOK_H

#include <iostream>

namespace Hook {
    void init();
    void cleanup();
    bool handleInput(int key, const char* type);
};

#endif
