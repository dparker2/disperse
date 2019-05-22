#ifndef TAILOR_H
#define TAILOR_H

namespace Tailor {
    enum class Action { NONE, UP, DOWN, LEFT, RIGHT, FULL, CENTER };
    enum class Section { NONE, HALF, THIRD, FOURTH };

    typedef struct command {
        Action action;
        Section section;
        bool lockDimension;
        void* windowHandle;
    } command;

    void alter(command);
};

#endif
