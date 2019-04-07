#ifndef HOTKEYS_H
#define HOTKEYS_H

#include "constants.h"
#include <vector>
#include <map>

typedef std::vector<UINT> VkModifiers;
typedef std::map<UINT, const char*> VkNames;

class Hotkeys {
public:
    Hotkeys(HWND);
    ~Hotkeys();
    void handleInput(LPARAM);

private:
    //std::vector<Hotkey*> hotkeys;
    RAWKEYBOARD rawInputToKeyboard(HRAWINPUT rawInput);
    bool shouldHandleKey(UINT, UINT);
    void handleKey(UINT);
    void fullscreen(HWND, RECT);
    void center(HWND, RECT, RECT);
    void translate(HWND, RECT, RECT);
    void split(HWND, RECT, RECT);
    bool isPressed(VkModifiers);
    bool isPressed(UINT);
    bool isActionKey(UINT);
    const char* currPressed(VkNames);
    UINT prevAction;
    UINT currInput;
    VkModifiers activation;
    VkModifiers directionLock;
    VkNames sections;
    VkNames actions; 
    HHOOK keyboardHook;
};

#endif