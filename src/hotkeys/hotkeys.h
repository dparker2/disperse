#ifndef HOTKEYS_H
#define HOTKEYS_H

#include "hotkey.h"
#include <vector>

class Hotkeys {
public:
    Hotkeys();
    ~Hotkeys();
    void handle(int);

private:
    std::vector<Hotkey*> hotkeys;
};

#endif