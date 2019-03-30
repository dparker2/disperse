#include "iostream"

#include "app/app.h"
#include "hotkeys/hotkeys.h"

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow) {
    App* app = new App();

    if (!app->createWindow(hThisInstance, nCmdShow)) {
        delete app;
        return 0;
    }

    Hotkeys* hotkeys = new Hotkeys();    

    MSG msg;
    while (GetMessage (&msg, NULL, 0, 0)) {
        if (msg.message == WM_HOTKEY)
            hotkeys->handle(msg.wParam);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    delete app;
    delete hotkeys;

    return msg.wParam;
}
