#include "hook.h"
#include "../worker/worker.h"
#include "../hotkeys/hotkeys.h"
#include "../windows/api.h"

#include <cstring>

Worker* worker;

void Hook::init() {
    worker = new Worker();
    Hotkeys::load();
    API::attachHook(&Hook::handleInput);
}

void Hook::cleanup() {
    delete worker;
}

bool Hook::handleInput(int key, const char* type) {
    static int prevKey;
    Tailor::command command;
    bool isUp = strcmp(type, "up") == 0;

    if (prevKey == key) {
        prevKey = isUp ? 0 : key;  // reset prevKey on release
        return true;  // same key as before, ignore
    } else if (isUp) {
        return true;  // key was being held down, ignore keyup
    } else if (!Hotkeys::shouldHandle()) {
        return true;  // activation keys not being held
    }

    command.action = Hotkeys::getAction(key);
    if (command.action == Tailor::Action::NONE)
        return true;  // not an action
    command.section = Hotkeys::getSection();
    command.lockDimension = Hotkeys::shouldLockDimension();
    command.windowHandle = API::getForegroundWindow();
    worker->process(command);

    prevKey = key;  // store key for ignoring
    return false;
}
