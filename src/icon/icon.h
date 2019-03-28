#ifndef ICON_H
#define ICON_H

#include "constants.h"

class Icon {
public:
    Icon(HWND);
    void add();
    void remove();
    void initMenu();
    UINT openMenu(HWND);

private:
    NOTIFYICONDATA iconData;
    HMENU hMenu;
};

#endif
