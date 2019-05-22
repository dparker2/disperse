#ifndef API_H
#define API_H

namespace API {
    int startLoop();
    void attachHook(bool shouldSuppress(int, const char*));
    void sleep(int ms);
    bool isAsyncKeyPressed(int vkCode);
    void* getForegroundWindow();
    typedef struct rect {
        int x;
        int y;
        int width;
        int height;
    } rect;
    rect getMonitorRectByHwnd(void* hwnd);
    rect getWindowRectByHwnd(void* hwnd);
    void resize(void* hwnd, int width, int height);
    void translate(void* hwnd, int x, int y);
};

#endif
