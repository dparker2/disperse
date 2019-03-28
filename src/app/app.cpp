#include "app.h"
#include <iostream>

App::~App() {
    if (this->icon != nullptr) {
        delete this->icon;
    }
}

bool App::createWindow(HINSTANCE hThisInstance, int nCmdShow) {
    WNDCLASSEX winclass = this->makeWindowClass(hThisInstance);
    
    bool ok = RegisterClassEx(&winclass);
    if (!ok)
        return false;
    
    CreateWindowEx(
        0,                   /* Extended possibilites for variation */
        windowName,          /* Classname */
        windowName,          /* Title Text */
        WS_OVERLAPPEDWINDOW, /* default window */
        CW_USEDEFAULT,       /* Windows decides the position */
        CW_USEDEFAULT,       /* where the window ends up on the screen */
        544,                 /* The programs width */
        375,                 /* and height in pixels */
        HWND_DESKTOP,        /* The window is a child-window to desktop */
        NULL,                /* No menu */
        hThisInstance,       /* Program Instance handler */
        this                 /* Pass in app pointer so callback can access it */
    );

    if (this->hwnd == nullptr)
        return false;
    
    ShowWindow(this->hwnd, nCmdShow);

    return true;
}

WNDCLASSEX App::makeWindowClass(HINSTANCE hThisInstance) {
    /* The Window structure */
    WNDCLASSEX winclass;
    winclass.hInstance = hThisInstance;
    winclass.lpszClassName = windowName;
    winclass.lpfnWndProc = App::thisWindowProc;
    winclass.style = CS_DBLCLKS;                 /* Catch double-clicks */
    winclass.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    winclass.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
    winclass.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
    winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    winclass.lpszMenuName = NULL;                 /* No menu */
    winclass.cbClsExtra = 0;                      /* No extra bytes after the window class */
    winclass.cbWndExtra = 0;                      /* structure or the window instance */
    winclass.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));
    return winclass;
}

bool App::isVisible() {
    return IsWindowVisible(this->hwnd);
}

void App::hide() {
    ShowWindow(this->hwnd, SW_HIDE);
}

void App::show() {
    ShowWindow(this->hwnd, SW_SHOW);
}

void App::toFront() {
    SetForegroundWindow(this->hwnd);
}

LRESULT CALLBACK App::thisWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    App* _this;

    if (message == WM_NCCREATE) {
        CREATESTRUCT *cs = (CREATESTRUCT*) lParam; 
        _this = (App*) cs->lpCreateParams;

        SetLastError(0);
        if (SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) _this) == 0 && GetLastError() != 0) {
            return FALSE;
        } else {
            _this->hwnd = hwnd;
            _this->icon = new Icon(hwnd);
            return TRUE;
        }
    } else {
        _this = (App*) GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (_this) {
        return _this->WindowProc(message, wParam, lParam);
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT App::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    if (message==WM_TASKBAR && !this->isVisible()) {
        this->hide();
        return 0;
    }

    switch (message) {
        case WM_ACTIVATE:
            this->icon->add();
            break;

        case WM_CREATE:
            this->hide();
            this->icon->initMenu();
            break;

        case WM_SYSCOMMAND:
            /*In WM_SYSCOMMAND messages, the four low-order bits of the wParam parameter 
            are used internally by the system. To obtain the correct result when testing the value of wParam, 
            an application must combine the value 0xFFF0 with the wParam value by using the bitwise AND operator.*/
            switch(wParam & 0xFFF0) {
                case SC_MINIMIZE:
                case SC_CLOSE:  
                    this->hide() ;
                    return 0;
            }
            break;

        case WM_SYSICON:
            if (wParam == TRAY_ICON_ID) {
                this->toFront();
            }

            if (lParam == WM_LBUTTONUP) {
                this->show();
            } else if (lParam == WM_RBUTTONDOWN) {
                this->toFront();

                UINT clicked = this->icon->openMenu(this->hwnd);
                SendMessage(this->hwnd, WM_NULL, 0, 0); // send benign message to window to make sure the menu goes away.
                if (clicked == TRAY_EXIT_ID) {
                    // quit the application.
                    this->icon->remove();
                    PostQuitMessage(0);
                }
            }
            break;

        case WM_NCHITTEST: {
            UINT uHitTest = DefWindowProc(this->hwnd, WM_NCHITTEST, wParam, lParam);
            if (uHitTest == HTCLIENT)
                return HTCAPTION;
            else
                return uHitTest;
        }

        case WM_CLOSE:
            this->hide();
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(this->hwnd, message, wParam, lParam);
}