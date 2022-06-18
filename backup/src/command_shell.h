#pragma once

#include <string>
#include "command_list.h"

namespace shell {

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

    KEYBOARD keyboard_input(char& c);

    void flush_print(const std::string& to_print);

    void execute_command(const command_list& cst, const std::string& command);

    void clear_buffer();
    
    void run(const command_list& cst);
}
