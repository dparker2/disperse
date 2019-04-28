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
    void fullscreen();
    void center();
    void split(UINT, const char*);
    void translate(UINT);
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
    HWND activeWindow;
    RECT activeRect;
    RECT monitorRect;
};

#endif