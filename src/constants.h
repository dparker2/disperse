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

const UINT ONE      = 0x31;
const UINT TWO      = 0x32;
const UINT THREE    = 0x33;
const UINT FOUR     = 0x34;
const UINT C        = 0x43;
const UINT F        = 0x46;
const UINT LEFT     = VK_LEFT;
const UINT UP       = VK_UP;
const UINT RIGHT    = VK_RIGHT;
const UINT DOWN     = VK_DOWN;

#endif
