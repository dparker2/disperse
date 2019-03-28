#ifndef CONSTANTS_H
#define CONSTANTS_H
#define UNICODE 1

#include <windows.h>
#include <shellapi.h>

const LPCWSTR windowName = (LPCWSTR)"Some App Name";

const UINT ICO1 = 101;

const UINT TRAY_ICON_ID = 1001;
const UINT TRAY_EXIT_ID = 1002;

const UINT WM_SYSICON = WM_USER + 1;
const UINT WM_TASKBAR = RegisterWindowMessageA("TaskbarCreated");

#endif
