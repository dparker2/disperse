#include "hotkey.h"
#include <iostream>

Hotkey::Hotkey(
    int keyID,
    UINT fsModifiers,
    UINT vKeyCode,
    float xStart, 
    float yStart,
    float width,
    float height
) {
    if (!RegisterHotKey(NULL, keyID, fsModifiers, vKeyCode))
        throw GetLastError();
    this->xStart = xStart;
    this->yStart = yStart;
    this->width = width;
    this->height = height;
}

void Hotkey::handle() {
    HWND selected = GetForegroundWindow();
    if (selected == NULL) {
        return;
    }
    HMONITOR monitor = MonitorFromWindow(selected, MONITOR_DEFAULTTONEAREST);
    MONITORINFO monitorInfo;
    monitorInfo.cbSize = sizeof(monitorInfo);
    if (!GetMonitorInfo(monitor, &monitorInfo))
        throw GetLastError();
    RECT monitorRect = monitorInfo.rcWork;
    Dimensions dims = calcDimensions(monitorRect);
    if (!SetWindowPos(selected, NULL, dims.x, dims.y, dims.width, dims.height, SWP_NOREPOSITION))
        throw GetLastError();
}

Dimensions Hotkey::calcDimensions(RECT monitorRect) {
    int monitorHLength = monitorRect.right - monitorRect.left;
    int monitorVLength = monitorRect.bottom - monitorRect.top;
    Dimensions dimensions;
    // ((MAX - MIN) * percent) + MIN = pos
    dimensions.x = (monitorHLength * this->xStart) + monitorRect.left;
    dimensions.y = (monitorVLength * this->yStart) + monitorRect.top;
    // (MAX - MIN) * percent = size
    dimensions.width = monitorHLength * this->width;
    dimensions.height = monitorVLength * this->width;
    return dimensions;
}