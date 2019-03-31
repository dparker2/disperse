#ifndef APP_H
#define APP_H

#include "constants.h"
#include "icon/icon.h"
#include "hotkeys/hotkeys.h"

class App {
public:
    ~App();
    bool createWindow(HINSTANCE, int);
    bool isVisible();
    void hide();
    void show();
    void toFront();

private:
    static LRESULT CALLBACK thisWindowProc(HWND, UINT, WPARAM, LPARAM);
    LRESULT WindowProc(UINT, WPARAM, LPARAM);
    WNDCLASSEX makeWindowClass(HINSTANCE);
    HWND hwnd;
    Icon* icon;
    Hotkeys* hotkeys;
};

#endif