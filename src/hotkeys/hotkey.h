#ifndef HOTKEY_H
#define HOTKEY_H

#include "constants.h"

struct Dimensions {
    int x;
    int y;
    int width;
    int height;
};

class Hotkey {
public:
    Hotkey(int, UINT, UINT, float, float, float, float);
    void handle();

private:
    float xStart; 
    float yStart;
    float width;
    float height;
    Dimensions calcDimensions(RECT);
};

#endif