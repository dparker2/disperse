#include "../windows/api.h"
#include "hotkeys.h"

#include <iostream>
#include <utility>
#include <vector>

std::vector<int> activation;

typedef std::vector<std::pair<Tailor::Section, int>> Sections;
Sections sections(3);

typedef std::vector<std::pair<Tailor::Action, int>> Actions;
Actions actions(6);

int directionLock;

void Hotkeys::load() {
    // Eventually read these from a settings file
    activation.push_back(0x11);  // CTRL
    activation.push_back(0x12);  // ALT

    sections[0] = { Tailor::Section::HALF, 0x32 };  // 2
    sections[1] = { Tailor::Section::THIRD, 0x33 };  // 3
    sections[2] = { Tailor::Section::FOURTH, 0x34 };  // 4

    actions[0] = { Tailor::Action::LEFT, 0x25 };
    actions[1] = { Tailor::Action::UP, 0x26 };
    actions[2] = { Tailor::Action::RIGHT, 0x27 };
    actions[3] = { Tailor::Action::DOWN, 0x28 };
    actions[4] = { Tailor::Action::FULL, 0x46 };  // F
    actions[5] = { Tailor::Action::CENTER, 0x43 };  // C

    directionLock = 0x31;  // 1
}

bool Hotkeys::shouldHandle() {
    std::vector<int>::iterator it;
    for (it = activation.begin(); it != activation.end(); ++it) {
        if (!API::isAsyncKeyPressed(*it)) return false;
    }
    return true;
}

Tailor::Action Hotkeys::getAction(int key) {
    Actions::iterator it;
    for (it = actions.begin(); it != actions.end(); ++it) {
        if (key == it->second) {
            return it->first;
        }
    }
    return Tailor::Action::NONE;
}

Tailor::Section Hotkeys::getSection() {
    Sections::iterator it;
    for (it = sections.begin(); it != sections.end(); ++it) {
        if (API::isAsyncKeyPressed(it->second)) {
            return it->first;
        }
    }
    return Tailor::Section::NONE;
}

bool Hotkeys::shouldLockDimension() {
    return API::isAsyncKeyPressed(directionLock);
}
