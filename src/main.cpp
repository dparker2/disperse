#include "iostream"

#include "app/app.h"

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow) {
    App* app = new App();

    if (!app->createWindow(hThisInstance, nCmdShow))
        return 0;

    MSG messages;
    while (GetMessage (&messages, NULL, 0, 0)) {
        TranslateMessage(&messages);
        DispatchMessage(&messages);  // App::thisWindowProc
    }

    delete app;

    return messages.wParam;
}
