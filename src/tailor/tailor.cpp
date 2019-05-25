#include "tailor.h"
#include "../windows/api.h"

#include <algorithm>

void* hwnd;
API::rect monitor;
API::rect window;

void fullscreen();
void center();
void translate(Tailor::Action);
void split(Tailor::Action, float, bool);

void Tailor::alter(Tailor::command command) {
    float percent;
    static Action prevAction = Action::NONE;
    bool repeatAction = prevAction == command.action;
    prevAction = repeatAction ? Action::NONE : command.action;
    hwnd = command.windowHandle;
    monitor = API::getMonitorRectByHwnd(hwnd);
    window = API::getWindowRectByHwnd(hwnd);

    switch(command.action) {
    case Action::FULL:
        fullscreen();
        return;
    case Action::CENTER:
        center();
        return;
    default:
        switch(command.section) {
        case Section::NONE:
            translate(command.action);
            return;
        case Section::HALF:
            percent = 1.0/2.0;
            break;
        case Section::THIRD:
            percent = repeatAction ? 2.0/3.0 : 1.0/3.0;
            break;
        case Section::FOURTH:
            percent = repeatAction ? 3.0/4.0 : 1.0/4.0;
            break;
        }
        split(command.action, percent, command.lockDimension);
    }
}

void fullscreen() {
    API::translate(hwnd, monitor.x, monitor.y);
    API::resize(hwnd, monitor.width, monitor.height);
}

void center() {
    int x = (monitor.width / 2) - (window.width / 2);
    int y = (monitor.height / 2) - (window.height / 2);
    API::translate(hwnd, x, y);
}

void translate(Tailor::Action action) {
    int x = window.x;
    int y = window.y;
    switch(action) {
    case Tailor::Action::LEFT:
        x = std::max(x - window.width, monitor.x);
        break;
    case Tailor::Action::UP:
        y = std::max(y - window.height, monitor.y);
        break;
    case Tailor::Action::RIGHT:
        x = std::min(x + window.width, monitor.x + monitor.width - window.width);
        break;
    case Tailor::Action::DOWN:
        y = std::min(y + window.height, monitor.y + monitor.height - window.height);
        break;
    }
    API::translate(hwnd, x, y);
}

void splitLeft(int& x, int& y, int& width, int& height, float percent, bool lockDir) {
    x = monitor.x;
    width = monitor.width * percent;
    if (!lockDir) {
        y = monitor.y;
        height = monitor.height;
    }
}

void splitUp(int& x, int& y, int& width, int& height, float percent, bool lockDir) {
    y = monitor.y;
    height = monitor.height * percent;
    if (!lockDir) {
        x = monitor.x;
        width = monitor.width;
    }
}

void splitRight(int& x, int& y, int& width, int& height, float percent, bool lockDir) {
    width = monitor.width * percent;
    x = monitor.x + monitor.width - width;
    if (!lockDir) {
        y = monitor.y;
        height = monitor.height;
    }
}

void splitDown(int& x, int& y, int& width, int& height, float percent, bool lockDir) {
    height = monitor.height * percent;
    y = monitor.y + monitor.height - height;
    if (!lockDir) {
        x = monitor.x;
        width = monitor.width;
    }
}

void split(Tailor::Action action, float percent, bool lockDir) {
    int x = window.x;
    int y = window.y;
    int width = window.width;
    int height = window.height;
    switch(action) {
    case Tailor::Action::LEFT:
        splitLeft(x, y, width, height, percent, lockDir);
        break;
    case Tailor::Action::UP:
        splitUp(x, y, width, height, percent, lockDir);
        break;
    case Tailor::Action::RIGHT:
        splitRight(x, y, width, height, percent, lockDir);
        break;
    case Tailor::Action::DOWN:
        splitDown(x, y, width, height, percent, lockDir);
        break;
    }
    API::translate(hwnd, x, y);
    API::resize(hwnd, width, height);
}
