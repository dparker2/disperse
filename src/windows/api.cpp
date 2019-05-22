#include "api.h"
#include "api/util.h"
#include <windows.h>
#include <iostream>
#include <bitset>

int API::startLoop() {
    MSG msg;
    while (GetMessage (&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

bool (*handleInput)(int, const char*) = NULL;
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0) {  // Required by windows API
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    int vkCode = ((KBDLLHOOKSTRUCT*)lParam)->vkCode;
    bool shouldContinue = handleInput(vkCode, wParam == WM_KEYUP || wParam == WM_SYSKEYUP ? "up" : "down");
    if (shouldContinue) {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    return TRUE;  // Swallow input
}

void API::attachHook(bool onInput(int, const char*)) {
    handleInput = onInput;
    callWinAPI(SetWindowsHookEx, (
        WH_KEYBOARD_LL,
        LowLevelKeyboardProc,
        NULL,
        0
    ));
}

void API::sleep(int ms) {
    Sleep(ms);
}

bool API::isAsyncKeyPressed(int vkCode) {
    return (GetAsyncKeyState(vkCode) & 32768) > 0;
}

void* API::getForegroundWindow() {
    return (void*)GetForegroundWindow();
}

API::rect API::getMonitorRectByHwnd(void* hwnd) {
    HMONITOR monitor = MonitorFromWindow((HWND)hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO monitorInfo;
    monitorInfo.cbSize = sizeof(monitorInfo);
    callWinAPI(GetMonitorInfo, (monitor, &monitorInfo));
    RECT workRect = monitorInfo.rcWork;
    API::rect rect;
    rect.x = workRect.left;
    rect.y = workRect.top;
    rect.width = workRect.right - workRect.left;
    rect.height = workRect.bottom - workRect.top;
    return rect;
}

API::rect API::getWindowRectByHwnd(void* hwnd) {
    RECT windowRect;
    callWinAPI(GetWindowRect, ((HWND)hwnd, &windowRect));
    API::rect rect;
    rect.x = windowRect.left;
    rect.y = windowRect.top;
    rect.width = windowRect.right - windowRect.left;
    rect.height = windowRect.bottom - windowRect.top;
    return rect;
}

void API::resize(void* hwnd, int width, int height) {
    callWinAPI(SetWindowPos, (
        (HWND)hwnd,
        NULL,
        0,
        0,
        width,
        height,
        SWP_NOZORDER | SWP_NOMOVE
    ));
}

void API::translate(void* hwnd, int x, int y) {
    callWinAPI(SetWindowPos, (
        (HWND)hwnd,
        NULL,
        x,
        y,
        0,
        0,
        SWP_NOZORDER | SWP_NOSIZE
    ));
}
