#include "hotkeys.h"
#include <iostream>

typedef struct Dimensions {
    int x;
    int y;
    int width;
    int height;
} Dimensions;

typedef struct AreaSize {
    float primary;
    float secondary;
    float pos;
} AreaSize;

RECT getMonitorRect(HWND hWnd);
Dimensions calcNewDimensions(RECT monitorRect, float xStart, float yStart, float width, float height);
void reposition(HWND hWnd, Dimensions dims);

VkNames initSections() {
    VkNames vks;
    vks[TWO] = "half";
    vks[THREE] = "third";
    vks[FOUR] = "fourth";
    return vks;
}

VkNames initActions() {
    VkNames vks;
    vks[UP] = "up";
    vks[DOWN] = "down";
    vks[LEFT] = "left";
    vks[RIGHT] = "right";
    vks[F] = "full";
    vks[C] = "center";
    return vks;
}

void registerKeyboardInputs(HWND hWnd) {
    RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = 0x01; 
    Rid[0].usUsage = 0x06; 
    Rid[0].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;
    Rid[0].hwndTarget = hWnd;
    if (!RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])))
        throw GetLastError();
}

Hotkeys::Hotkeys(HWND hWnd) {
    this->prevAction = 0;
    this->activation = VkModifiers{VK_MENU, VK_CONTROL};
    this->directionLock = VkModifiers{ONE};
    this->sections = initSections();
    this->actions = initActions();
    registerKeyboardInputs(hWnd);
}

Hotkeys::~Hotkeys() {

}

void Hotkeys::handleInput(LPARAM lParam) {
    RAWKEYBOARD keyboard = this->rawInputToKeyboard((HRAWINPUT)lParam);
    UINT vKeyCode = keyboard.VKey;
    if (this->shouldHandleKey(vKeyCode, keyboard.Message)) {
         this->handleKey(vKeyCode);
    }
}

bool Hotkeys::shouldHandleKey(UINT vKeyCode, UINT message) {
    if (!this->isActionKey(vKeyCode) || !this->isPressed(this->activation)) {
        return false;  // Don't care if its not an action key or the activation keys aren't pressed
    }

    if (this->currInput == vKeyCode) {
        if (message == WM_KEYDOWN || message == WM_SYSKEYDOWN) {
            return false;  // If same key as before and keydown, ignore
        } else if (message == WM_KEYUP || message == WM_SYSKEYUP) {
            this->currInput = 0;  // Stop ignoring on keyup
            return false;
        }
    }
    
    // New key, ignore future keydowns
    this->currInput = vKeyCode;
    return true;
}

RAWKEYBOARD Hotkeys::rawInputToKeyboard(HRAWINPUT rawInput) {
    UINT dwSize;

    GetRawInputData(rawInput, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
    LPBYTE lpb = new BYTE[dwSize];
    if (lpb == NULL)
        throw GetLastError();

    if (GetRawInputData(rawInput, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize )
         OutputDebugString (TEXT("GetRawInputData does not return correct size !\n")); 

    RAWINPUT* raw = (RAWINPUT*)lpb;
    return raw->data.keyboard;
}

bool Hotkeys::isPressed(VkModifiers modifiers) {
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

bool Hotkeys::isActionKey(UINT vkCode) {
    return this->actions.count(vkCode) == 1;
}

const char* Hotkeys::currPressed(VkNames vkNames) {
    VkNames::iterator it;
    for (it = vkNames.begin(); it != vkNames.end(); ++it) {
        if (this->isPressed(it->first)) {
            return it->second;
        }
    }
    return 0;
}

void Hotkeys::fullscreen(HWND selected, RECT monitorRect) {
    Dimensions dimensions = {
        monitorRect.left,
        monitorRect.top,
        monitorRect.right - monitorRect.left,
        monitorRect.bottom - monitorRect.top
    };
    reposition(selected, dimensions);
}

void Hotkeys::handleKey(UINT vkCode) {
    HWND selected = GetForegroundWindow();
    if (selected == NULL) {
        return;
    }
    RECT selectedRect;
    if (!GetWindowRect(selected, &selectedRect)) {
        return;
    }
    RECT monitorRect = getMonitorRect(selected);

    if (vkCode == F) {
        this->fullscreen(selected, monitorRect);
        return;
    }

    Dimensions newDimensions;
    AreaSize areaSize;
    bool flipArea = vkCode == this->prevAction;
    bool up = vkCode == UP;
    bool down = vkCode == DOWN;
    bool left = vkCode == LEFT;
    bool right = vkCode == RIGHT;
    bool lockDirection = isPressed(this->directionLock);
    bool vertical = up || down;
    bool horizontal = left || right;

    // TODO refactor this function to:
    //  2 - Call different functions for
    //      - fullscreen (dont care about other keys, just resize)
    //      - center (dont care about other keys, just reposition)
    //      - direction
    const char* section = this->currPressed(this->sections);
    if (section == this->sections[TWO]) {
        areaSize.primary = 0.5;
    } else if (section == this->sections[THREE]) {
        areaSize.primary = flipArea ? 0.667 : 0.333;
    } else if (section == this->sections[FOUR]) {
        areaSize.primary = flipArea ? 0.75 : 0.25;
    } else {
        return;
    }

    if (lockDirection) {
        if (vertical) {
            areaSize.pos = (float)selectedRect.left / (float)(monitorRect.right - monitorRect.left);
            areaSize.secondary = (float)(selectedRect.right - selectedRect.left) / (float)(monitorRect.right - monitorRect.left);
        } else {
            areaSize.pos = (float)selectedRect.top / (float)(monitorRect.bottom - monitorRect.top);
            areaSize.secondary = (float)(selectedRect.bottom - selectedRect.top) / (float)(monitorRect.bottom - monitorRect.top);
        }
    } else {
        areaSize.pos = 0;
        areaSize.secondary = 1;
    }

    if (up) {
        newDimensions = calcNewDimensions(monitorRect, areaSize.pos, 0, areaSize.secondary, areaSize.primary);
    } else if (down) {
        newDimensions = calcNewDimensions(monitorRect, areaSize.pos, (1 - areaSize.primary), areaSize.secondary, areaSize.primary);
    } else if (left) {
        newDimensions = calcNewDimensions(monitorRect, 0, areaSize.pos, areaSize.primary, areaSize.secondary);
    } else if (right) {
        newDimensions = calcNewDimensions(monitorRect, (1 - areaSize.primary), areaSize.pos, areaSize.primary, areaSize.secondary);
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