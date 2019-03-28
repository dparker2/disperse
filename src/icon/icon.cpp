#include "icon.h"
#include <iostream>

Icon::Icon(HWND hWnd) {
    TCHAR szTIP[64] = TEXT("Some type of tooltip");
    memset(&this->iconData, 0, sizeof(NOTIFYICONDATA));

    this->iconData.cbSize = sizeof(this->iconData);
    this->iconData.hWnd = hWnd;
    this->iconData.uID = TRAY_ICON_ID;
    this->iconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    this->iconData.uCallbackMessage = WM_SYSICON;
    this->iconData.hIcon = (HICON)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
    strncpy((char*)this->iconData.szTip, (char*)szTIP, sizeof(szTIP));
}

void Icon::add() {
    Shell_NotifyIcon(NIM_ADD, &this->iconData);
}

void Icon::remove() {
    Shell_NotifyIcon(NIM_DELETE, &this->iconData);
}

void Icon::initMenu() {
    this->hMenu = CreatePopupMenu();
    AppendMenu(this->hMenu, MF_STRING, TRAY_EXIT_ID,  TEXT("Exit The Demo"));
}

UINT Icon::openMenu(HWND hWnd) {
    POINT curPoint;
    GetCursorPos(&curPoint);

    return TrackPopupMenu(
        this->hMenu, 
        TPM_RETURNCMD | TPM_NONOTIFY,
        curPoint.x,
        curPoint.y,
        0,
        hWnd,
        NULL
    );
}