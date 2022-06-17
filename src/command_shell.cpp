#include "command_shell.h"
#ifndef WINDOWS_PLATFORM
#include "read_buf.h"
#else
#include <conio.h>
#endif 
#include <iostream>
#include <sstream>

static int pos = 0;

shell::KEYBOARD shell::keyboard_input(char& c) {
    c = _getch();
	if (c == 9)
		return shell::KEYBOARD::TAB;
	else if (c == 10 || c == 13)
		return shell::KEYBOARD::ENTER;
	else if (c == 127 || c == 8)
		return shell::KEYBOARD::BACKSPACE;
	else if (c == 3)
		return shell::KEYBOARD::EXIT;
#ifndef WINDOWS_PLATFORM
    else if (c == 27) {
        c = _getch();
        c = _getch();
        if (c == 65)
            return shell::KEYBOARD::UP;
        else if (c == 66)
            return shell::KEYBOARD::DOWN;
        else
            return shell::KEYBOARD::ERROR;
    }
#else
    else if (c == -32) {
        c = _getch();
        if (c == 72)
            return shell::KEYBOARD::UP;
        else if (c == 80)
            return shell::KEYBOARD::DOWN;
        else
            return shell::KEYBOARD::ERROR;
    }
#endif
    else
        return shell::KEYBOARD::ALPHA;
}


void shell::flush_print(const std::string& to_print) {
    std::string line;
    for (int i = 0; i < pos; ++i) line += '\b';
    line += to_print;
    std::string back_pos;
    for (int i = to_print.length(); i < pos; ++i) {
        line += ' ';
        back_pos += '\b';
    }
    line += back_pos;
    pos = to_print.length();
    std::cout << line << std::flush;
}

void shell::clear_buffer() {
    std::cout << ">>> " << std::flush;
    pos = 0;
}

void shell::execute_command(const command_list& cst, const std::string& command) {
    std::stringstream ss(command);
    std::string word;
    std::vector<std::string> command_para_list;
    while (ss >> word) {
        command_para_list.push_back(word);
    }
    if (command_para_list.empty()) {
        return;
    }
    if (cst.command_exist(command_para_list[0])) {
        std::cout << "Execute: " << command << std::endl;
    }
    else {
        std::cout << "Command not found." << std::endl;
    }
}

void shell::run(const command_list& cst) {
    std::vector<std::string> command_history = { "" }; 
    std::string current_command;
    int command_cursor = 1;
    shell::clear_buffer();
    while (true) {
        char c;
        shell::KEYBOARD key = shell::keyboard_input(c);
        if (key == shell::KEYBOARD::TAB) {
            std::string prefix = cst.longest_prefix(current_command);
            current_command = prefix.empty() ? current_command : prefix;
            flush_print(current_command);
        }
        else if (key == shell::KEYBOARD::ENTER) {
            std::cout << std::endl;
            execute_command(cst, current_command);
            command_history.push_back(current_command);
            command_cursor = command_history.size();
            current_command.clear();
            shell::clear_buffer();
        }
        else if (key == shell::KEYBOARD::BACKSPACE && !current_command.empty()) {
            current_command.pop_back();
            flush_print(current_command);
        }
        else if (key == shell::KEYBOARD::UP) {
            current_command = command_history[command_cursor > 0 ? --command_cursor : 0];
            flush_print(current_command);
        }
        else if (key == shell::KEYBOARD::DOWN) {
            int n = command_history.size();
            current_command = command_cursor < n - 1 ? command_history[command_cursor + 1] : "";
            command_cursor = command_cursor < n ? command_cursor + 1 : command_cursor;
            flush_print(current_command);
        }
        else if (key == shell::KEYBOARD::ALPHA){
            current_command.push_back(c);
            flush_print(current_command);
        }
        else if (key == shell::KEYBOARD::EXIT) {
            break;
        }
    }
}
