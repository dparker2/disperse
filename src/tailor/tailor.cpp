#include "tailor.h"
#include "../windows/api.h"

#include <iostream>

API::rect monitorRect;
void fullscreen(void*);
void center(void*);
void translate(void*);

void Tailor::alter(Tailor::command command) {
    monitorRect = API::getMonitorRectByHwnd(command.windowHandle);
    if (command.action == Action::FULL) {
        fullscreen(command.windowHandle);
    } else if (command.action == Action::CENTER) {
        center(command.windowHandle);
    }
}

void fullscreen(void* hwnd) {
    API::translate(hwnd, monitorRect.x, monitorRect.y);
    API::resize(hwnd, monitorRect.width, monitorRect.height);
}

void center(void* hwnd) {
    API::rect windowRect = API::getWindowRectByHwnd(hwnd);
    int x = (monitorRect.width / 2) - (windowRect.width / 2);
    int y = (monitorRect.height / 2) - (windowRect.height / 2);
    API::translate(hwnd, x, y);
}
