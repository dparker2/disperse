#ifndef HOTKEYS_H
#define HOTKEYS_H

#include "constants.h"
#include <vector>
#include <map>

typedef std::vector<UINT> vkModifiers;
typedef std::map<UINT, const char*> vkNames;

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
    bool isPressed(vkModifiers);
    bool isPressed(UINT);
    bool isActionKey(UINT);
    UINT prevAction;
    UINT prevInput;
    vkModifiers activation;
    vkModifiers directionLock;
    vkNames sections;
    vkNames actions; 
    HHOOK keyboardHook;
};

#endif