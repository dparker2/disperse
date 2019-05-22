#ifndef HOTKEYS_H
#define HOTKEYS_H

#include "../tailor/tailor.h"

namespace Hotkeys {
    void load();
    bool shouldHandle();
    Tailor::Action getAction(int key);
    Tailor::Section getSection();
    bool shouldLockDimension();
};

#endif
