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

    MSG msg;
    while (GetMessage (&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    delete app;

    return msg.wParam;
}
