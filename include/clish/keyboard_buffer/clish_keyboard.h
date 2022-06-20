#ifndef _CLISH_KEYBOARD_H
#define _CLISH_KEYBOARD_H

#ifndef WINDOWS_PLATFORM
#include "read_buf.h"
#else 
#include <conio.h>
#endif

namespace clish {

enum class KEYBOARD{
    ERROR,
    ALPHA,
    BACKSPACE,
    ENTER,
    TAB,
    UP,
    DOWN,
    EXIT
};

inline KEYBOARD keyboard_input(char& c) {
    c = _getch();
	if (c == 9)
		return KEYBOARD::TAB;
	else if (c == 10 || c == 13)
		return KEYBOARD::ENTER;
	else if (c == 127 || c == 8)
		return KEYBOARD::BACKSPACE;
	else if (c == 4)
		return KEYBOARD::EXIT;
#ifndef WINDOWS_PLATFORM
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
#else
    else if (c == -32) {
        c = _getch();
        if (c == 72)
            return KEYBOARD::UP;
        else if (c == 80)
            return KEYBOARD::DOWN;
        else
            return KEYBOARD::ERROR;
    }
#endif
    return KEYBOARD::ALPHA;
}

}

#endif
