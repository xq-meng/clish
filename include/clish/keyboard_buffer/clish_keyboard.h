#ifndef _CLISH_KEYBOARD_H
#define _CLISH_KEYBOARD_H

#ifndef WINDOWS_PLATFORM
#include "read_buf.h"
#else 
#include <conio.h>
#endif

namespace clish {

    enum class KEYBOARD {
        ALPHA,
        BACKSPACE,
        ENTER,
        ERROR,
        EXIT,
        INTERRUPT,
        TAB,
        UP,
        DOWN,
        UNSPECIFIED,
    };

#ifndef WINDOWS_PLATFORM
    KEYBOARD keyboard_input(char& c) {
        c = _getch();
        if (c == 3)
            return KEYBOARD::INTERRUPT;
        else if (c == 4)
            return KEYBOARD::EXIT;
        else if (c == 9)
            return KEYBOARD::TAB;
        else if (c == 10)
            return KEYBOARD::ENTER;
        else if (c == 127)
            return KEYBOARD::BACKSPACE;
        else if (c == 27) {
            c = _getch();
            c = _getch();
            if (c == 65)
                return KEYBOARD::UP;
            else if (c == 66)
                return KEYBOARD::DOWN;
            else
                return KEYBOARD::ERROR;
        }
        else if (c < 32)
            return KEYBOARD::UNSPECIFIED;
        return KEYBOARD::ALPHA;
    }
#else
    KEYBOARD keyboard_input(char& c) {
        c = _getch();
        if (c == 3)
            return KEYBOARD::INTERRUPT;
        else if (c == 4)
            return KEYBOARD::EXIT;
        else if (c == 9)
            return KEYBOARD::TAB;
        else if (c == 13)
            return KEYBOARD::ENTER;
        else if (c == 8)
            return KEYBOARD::BACKSPACE;
        else if (c == -32) {
            c = _getch();
            if (c == 72)
                return KEYBOARD::UP;
            else if (c == 80)
                return KEYBOARD::DOWN;
            else
                return KEYBOARD::ERROR;
        }
        else if (c < 32)
            return KEYBOARD::UNSPECIFIED;
        return KEYBOARD::ALPHA;
    }
#endif // WINDOWS_PLATFORM
    
} // namespace clish

#endif // _CLISH_KEYBOARD_H
