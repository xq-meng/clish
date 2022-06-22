#ifndef _CLISH_KEYBOARD_H
#define _CLISH_KEYBOARD_H

#ifndef WINDOWS_PLATFORM
#include "read_buf.h"
#else 
#include <conio.h>
#endif
#include <array>
#include <unordered_map>

namespace clish {

    enum class KEYBOARD {
        ALPHA,
        BACKSPACE,
        ENTER,
        ERROR,
        ESC,
        EXIT,
        INTERRUPT,
        TAB,
        UP,
        DOWN,
        RIGHT,
        LEFT,
        UNSPECIFIED,
    };

    typedef std::array<int, 3> key_ascii;

    /* Read up to 3 character without echo */
    inline void _getchs(key_ascii& vp) {
        vp[0] = _getch();
        vp[1] = 0;
        vp[2] = 0;
        int i = 1;
        while(_kbhit()) {
            vp[i++] = _getch();
        }
    }

    struct key_ascii_hash_fn {
        std::size_t operator()(const key_ascii& ka) const noexcept {
            return std::hash<int>()(ka[0]) ^ std::hash<int>()(ka[1]) ^ std::hash<int>()(ka[2]);
        }
    };

#ifdef WINDOWS_PLATFORM
    static std::unordered_map<key_ascii, KEYBOARD, key_ascii_hash_fn> ascii_map = {
        { {3, 0, 0},    KEYBOARD::INTERRUPT },
        { {4, 0, 0},    KEYBOARD::EXIT      },
        { {9, 0, 0},    KEYBOARD::TAB       },
        { {13, 0, 0},   KEYBOARD::ENTER     },
        { {27, 0, 0},   KEYBOARD::ESC       },
        { {8, 0, 0},    KEYBOARD::BACKSPACE },
        { {224, 72, 0}, KEYBOARD::UP        },
        { {224, 80, 0}, KEYBOARD::DOWN      },
        { {224, 77, 0}, KEYBOARD::RIGHT     },
        { {224, 75, 0}, KEYBOARD::LEFT      }
    };
#else 
    static std::unordered_map<key_ascii, KEYBOARD, key_ascii_hash_fn> ascii_map = {
        { {3, 0, 0},    KEYBOARD::INTERRUPT },
        { {4, 0, 0},    KEYBOARD::EXIT      },
        { {9, 0, 0},    KEYBOARD::TAB       },
        { {10, 0, 0},   KEYBOARD::ENTER     },
        { {27, 0, 0},   KEYBOARD::ESC       },
        { {127, 0, 0},  KEYBOARD::BACKSPACE },
        { {27, 91, 65}, KEYBOARD::UP        },
        { {27, 91, 66}, KEYBOARD::DOWN      },
        { {27, 91, 67}, KEYBOARD::RIGHT     },
        { {27, 91, 68}, KEYBOARD::LEFT      }
    };
#endif 

    inline KEYBOARD keyboard_input(char& c) {
        key_ascii vc;
        _getchs(vc);
        c = vc[0];
        if (ascii_map.count(vc)) 
            return ascii_map[vc];
        return (c < 32) ? KEYBOARD::UNSPECIFIED : KEYBOARD::ALPHA;
    }

} // namespace clish

#endif // _CLISH_KEYBOARD_H
