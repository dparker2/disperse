#ifndef HOTKEYS_H
#define HOTKEYS_H

#include "constants.h"
#include <vector>

typedef std::vector<UINT> vkModifiers;

typedef struct Section {
    UINT half;
    UINT third;
    UINT fourth;
} Section;

typedef struct Direction {
    UINT left;
    UINT right;
    UINT up;
    UINT down;
} Direction;

class Hotkeys {
public:
    Hotkeys(HWND);
    ~Hotkeys();
    void handle(LPARAM);

private:
    //std::vector<Hotkey*> hotkeys;
    bool isPressed(vkModifiers);
    bool isPressed(UINT);
    bool isDirectionKey(UINT);
    void handleKey(UINT);
    UINT prevAction;
    UINT prevInput;
    vkModifiers activation;
    vkModifiers directionLock;
    Section section;
    Direction direction; 
    HHOOK keyboardHook;
};

#endif