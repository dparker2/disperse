#include "hotkeys.h"
#include <iostream>

typedef struct Dimensions {
    int x;
    int y;
    int width;
    int height;
} Dimensions;

RECT getMonitorRect(HWND hWnd);
Dimensions calcNewDimensions(RECT monitorRect, float xStart, float yStart, float width, float height);
void reposition(HWND hWnd, Dimensions dims);

Hotkeys::Hotkeys(HWND hWnd) {
    this->prevAction = 0;
    this->activation = vkModifiers{VK_MENU, VK_SHIFT};
    this->directionLock = vkModifiers{Z};
    Section section;
    section.half = TWO;
    section.third = THREE;
    section.fourth = FOUR;
    this->section = section;
    Direction direction;
    direction.up = UP;
    direction.down = DOWN;
    direction.left = LEFT;
    direction.right = RIGHT;
    this->direction = direction;
    RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = 0x01; 
    Rid[0].usUsage = 0x06; 
    Rid[0].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;
    Rid[0].hwndTarget = hWnd;
    if (!RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])))
        throw GetLastError();
}

Hotkeys::~Hotkeys() {

}

void Hotkeys::handle(LPARAM lParam) {
    HRAWINPUT rawInput = (HRAWINPUT)lParam;
    UINT dwSize;

    GetRawInputData(rawInput, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
    LPBYTE lpb = new BYTE[dwSize];
    if (lpb == NULL)
        return;

    if (GetRawInputData(rawInput, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize )
         OutputDebugString (TEXT("GetRawInputData does not return correct size !\n")); 

    RAWINPUT* raw = (RAWINPUT*)lpb;

    if (!this->isDirectionKey(raw->data.keyboard.VKey)) {
        return; // Don't care if its not direction key
    }

    if (this->prevInput == raw->data.keyboard.VKey) {
        if (raw->data.keyboard.Message == WM_KEYDOWN || raw->data.keyboard.Message == WM_SYSKEYDOWN) {
            return;  // If same key as before and keydown, ignore
        } else if (raw->data.keyboard.Message == WM_KEYUP || raw->data.keyboard.Message == WM_SYSKEYUP) {
            this->prevInput = 0;  // Stop ignoring on keyup
            return;
        }
    }
    
    // New key, ignore future keydowns
    this->prevInput = raw->data.keyboard.VKey;

    if (this->isPressed(this->activation)) {
        this->handleKey(raw->data.keyboard.VKey);
    }
}

bool Hotkeys::isPressed(vkModifiers modifiers) {
    std::vector<UINT>::iterator it;
    for (it = modifiers.begin(); it != modifiers.end(); ++it) {
        if (!GetAsyncKeyState(*it))
            return false;
    }
    return true;
}

bool Hotkeys::isPressed(UINT vkCode) {
    return (bool)GetAsyncKeyState(vkCode);
}

bool Hotkeys::isDirectionKey(UINT vkCode) {
    return 
        vkCode == this->direction.up ||
        vkCode == this->direction.down ||
        vkCode == this->direction.left ||
        vkCode == this->direction.right;
}

void Hotkeys::handleKey(UINT vkCode) {
    //this->hotkeys[id]->handle();
    HWND selected = GetForegroundWindow();
    if (selected == NULL) {
        return;
    }
    RECT monitorRect = getMonitorRect(selected);
    Dimensions newDimensions;
    float areaSize;
    bool flipArea = vkCode == this->prevAction;

    if (isPressed(this->section.half)) {
        areaSize = 0.5;
    } else if (isPressed(this->section.third)) {
        areaSize = flipArea ? 0.667 : 0.333;
    } else if (isPressed(this->section.fourth)) {
        areaSize = flipArea ? 0.75 : 0.25;
    } else {
        return;
    }

    bool up = vkCode == this->direction.up;
    bool down = vkCode == this->direction.down;
    bool left = vkCode == this->direction.left;
    bool right = vkCode == this->direction.right;
    bool lockDirection = isPressed(this->directionLock);
    bool vertical = up || down;
    bool horizontal = left || right;

    if (lockDirection) {
        
    } else {

    }

    if (up) {
        newDimensions = calcNewDimensions(monitorRect, 0, 0, 1, areaSize);
    } else if (down) {
        newDimensions = calcNewDimensions(monitorRect, 0, (1 - areaSize), 1, areaSize);
    } else if (left) {
        newDimensions = calcNewDimensions(monitorRect, 0, 0, areaSize, 1);
    } else if (right) {
        newDimensions = calcNewDimensions(monitorRect, (1 - areaSize), 0, areaSize, 1);
    } else {
        return;
    }

    this->prevAction = flipArea ? 0 : vkCode;
    reposition(selected, newDimensions);
}

RECT getMonitorRect(HWND hWnd) {
    HMONITOR monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO monitorInfo;
    monitorInfo.cbSize = sizeof(monitorInfo);
    if (!GetMonitorInfo(monitor, &monitorInfo))
        throw GetLastError();
    return monitorInfo.rcWork;
}

Dimensions calcNewDimensions(RECT monitorRect, float xStart, float yStart, float width, float height) {
    int monitorHLength = monitorRect.right - monitorRect.left;
    int monitorVLength = monitorRect.bottom - monitorRect.top;
    Dimensions dimensions;
    // ((MAX - MIN) * percent) + MIN = pos
    dimensions.x = (monitorHLength * xStart) + monitorRect.left;
    dimensions.y = (monitorVLength * yStart) + monitorRect.top;
    // (MAX - MIN) * percent = size
    dimensions.width = monitorHLength * width;
    dimensions.height = monitorVLength * height;
    return dimensions;
}

void reposition(HWND hWnd, Dimensions dims) {
    if (!SetWindowPos(hWnd, NULL, dims.x, dims.y, dims.width, dims.height, SWP_NOREPOSITION))
        throw GetLastError();
}