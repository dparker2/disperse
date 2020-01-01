#include "api.h"
#include "api/util.h"

#include <windows.h>
#include <bitset>

#define TRAY_WINAPI 1
#include "api/tray.h"

void quitApp(struct tray_menu* item) {
    tray_exit();
}

void hideConsoleWindow(struct tray_menu* item) {
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);
}

void showConsoleWindow(struct tray_menu* item) {
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_SHOW);
}

int API::startLoop() {
    struct tray_menu menu[] = {
        {"Hide", 0, 0, hideConsoleWindow, NULL},
        {"Show", 0, 0, showConsoleWindow, NULL},
        {"Quit", 0, 0, quitApp, NULL},
        {NULL, 0, 0, NULL, NULL}
    };
    struct tray tray = {
        "icon.ico",
        "Disperse",
        menu
    };
    handleError(tray_init, (&tray), -1);
    while (tray_loop(1) == 0);
    tray_exit();
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
    handleError(SetWindowsHookEx, (
        WH_KEYBOARD_LL,
        LowLevelKeyboardProc,
        NULL,
        0
    ), 0);
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
    handleError(GetMonitorInfo, (monitor, &monitorInfo), 0);
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
    handleError(GetWindowRect, ((HWND)hwnd, &windowRect), 0);
    API::rect rect;
    rect.x = windowRect.left;
    rect.y = windowRect.top;
    rect.width = windowRect.right - windowRect.left;
    rect.height = windowRect.bottom - windowRect.top;
    return rect;
}

void API::resize(void* hwnd, int width, int height) {
    handleError(SetWindowPos, (
        (HWND)hwnd,
        NULL,
        0,
        0,
        width,
        height,
        SWP_NOZORDER | SWP_NOMOVE
    ), 0);
}

void API::translate(void* hwnd, int x, int y) {
    handleError(SetWindowPos, (
        (HWND)hwnd,
        NULL,
        x,
        y,
        0,
        0,
        SWP_NOZORDER | SWP_NOSIZE
    ), 0);
}
