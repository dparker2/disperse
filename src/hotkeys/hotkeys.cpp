#include "hotkeys.h"

Hotkeys::Hotkeys() {
    this->hotkeys = {
        new Hotkey(
            0,
            MOD_CONTROL | MOD_ALT,
            0x46,
            0,
            0,
            1,
            1
        )
    };
}

Hotkeys::~Hotkeys() {
    std::vector<Hotkey*>::iterator it;
    for (it = this->hotkeys.begin(); it != this->hotkeys.end(); ++it) {
        delete *it;
    }
}

void Hotkeys::handle(int id) {
    this->hotkeys[id]->handle();
}