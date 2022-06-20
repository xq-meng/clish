#pragma once 

#include "tsl/htrie_map.h"
#include "keyboard_buffer/clish_keyboard.h"

#include <iostream>
#include <functional>
#include <string>
#include <sstream>
#include <vector>

namespace clish {

template<typename T>
class command_manager {
private:
    tsl::htrie_map<char, T> cst;

public:
    T& get_val(const std::string& _command_str) {
        if (!command_exist(_command_str)) {
            throw "Command does not exist.";
        }
        return cst[_command_str];
    }

    bool command_exist(const std::string& _command_str) const {
        return this->cst.count(_command_str);
    }

    bool command_exist(std::string&& _command_str) const {
        return this->cst.count(_command_str);
    }

    void insert_command(std::string&& _command_str, const T& _fn) {
        this->cst.insert(_command_str, _fn);
    }

    void insert_command(const std::string& _command_str, const T& _fn) {
        this->cst.insert(_command_str, _fn);
    }

    std::vector<std::string> commands_with_prefix(const std::string& _prefix) const {
        auto prefix_range = this->cst.equal_prefix_range(_prefix);
        std::vector<std::string> ret;
        for (auto it = prefix_range.first; it != prefix_range.second; ++it) {
            ret.push_back(std::string(it.key()));
        }
        return ret;
    }

    std::vector<std::string> commands_with_prefix(std::string&& _prefix) const {
        auto prefix_range = this->cst.equal_prefix_range(_prefix);
        std::vector<std::string> ret;
        for (auto it = prefix_range.first; it != prefix_range.second; ++it) {
            ret.push_back(std::string(it.key()));
        }
        return ret;
    }

    std::string longest_prefix(const std::string& _prefix) const {
        std::string ret = _prefix;
        std::vector<std::string> candidates = this->commands_with_prefix(_prefix);
        if (candidates.empty()) 
        {
            return std::string();
        }
        std::string og = candidates[0];
        int i = ret.length();
        while ( std::all_of(candidates.begin(), candidates.end(), [i](std::string& _str) { return i < _str.length(); }) &&
            std::all_of(candidates.begin(), candidates.end(), [i, og](std::string& _str) { return _str[i] == og[i]; }))
            i++;
        return og.substr(0, i);
    }

    std::string longest_prefix(std::string&& _prefix) const {
        std::string ret = _prefix;
        std::vector<std::string> candidates = this->commands_with_prefix(std::move(_prefix));
        if (candidates.empty()) 
        {
            return std::string();
        }
        std::string og = candidates[0];
        int i = ret.length();
        while ( std::all_of(candidates.begin(), candidates.end(), [i](std::string& _str) { return i < _str.length(); }) &&
            std::all_of(candidates.begin(), candidates.end(), [i, og](std::string& _str) { return _str[i] == og[i]; }))
            i++;
        return og.substr(0, i);
    }
};

class clish {
private:
    command_manager<std::function<void(std::vector<std::string>)>> cm;

    std::vector<std::string> command_history = { "" };

    int pos = 0;
    int prev_hint_len = 0;

    inline std::string get_command(const std::string& line) {
        std::stringstream ss(line);
        std::string word;
        std::vector<std::string> command_para_list;
        while (ss >> word) {
            command_para_list.push_back(word);
        }
        return command_para_list.empty() ? "" : command_para_list[0];
    }

    inline void flush_print(const std::string& to_print) {
        // get command and parameter. command print in red or green, parameter print in default color.
        size_t space_pos = to_print.find(' ');
        std::string command_str = (space_pos == std::string::npos) ? to_print : to_print.substr(0, space_pos);
        std::string parameter_str = (space_pos == std::string::npos) ? "" : to_print.substr(space_pos, to_print.length() - space_pos);

        // move cursor to begin.
        std::string line(pos, '\b');
        std::cout << line << std::flush;

        // print command.
        std::string color_prefix = cm.command_exist(command_str) ? "\033[32m" : "\033[31m";
        std::cout << color_prefix << command_str << "\033[0m" << std::flush;

        // print parameters.
        std::cout << parameter_str << std::flush;

        // move cursor back to end of '_to_print'.
        line.clear();
        std::string back_pos;
        for (int i = to_print.length(); i < pos; ++i) {
            line += ' ';
            back_pos += '\b';
        }
        line += back_pos;
        std::cout << line << std::flush;

        // update cursor pos.
        pos = to_print.length();

        // print hints.
        std::string to_hint = parameter_str.empty() ? find_hint(to_print) : "";
        flush_hints(to_hint);
    }

    inline void flush_hints(const std::string& to_hint) {
        std::string line = to_hint;
        while (line.length() <= prev_hint_len) {
            line += ' ';
        }
        int temp_line_len = line.length();
        for (int i = 0; i < temp_line_len; i++) {
            line += '\b';
        }
        std::cout << "\033[3m" << line << "\033[0m" << std::flush;
        prev_hint_len = to_hint.length();
    }

    inline void clear_buffer() {
        std::cout << ">>> " << std::flush;
        pos = 0;
    }

    std::string find_hint(const std::string& _prefix_str) {
        if (_prefix_str.empty()) 
        {
            return std::string();
        }
        std::vector<std::string> candidates = cm.commands_with_prefix(_prefix_str);
        if (candidates.empty()) {
            return std::string();
        }
        int to_hint_id = -1;
        for (int i = command_history.size() - 1; i >= 0; --i) {
            if (std::find(candidates.begin(), candidates.end(), command_history[i]) != candidates.end()) {
                to_hint_id = i;
                break;
            }
        }
        std::string to_hint = (to_hint_id < 0) ? candidates[0] : command_history[to_hint_id];
        to_hint = to_hint.substr(_prefix_str.length(), to_hint.length() - _prefix_str.length());
        return to_hint;
    }

    void execute_command(const std::string& command) {
        std::stringstream ss(command);
        std::string command_str;
        ss >> command_str;
        std::string word;
        std::vector<std::string> parameters;
        while (ss >> word) {
            parameters.push_back(word);
        }
        if (command_str.empty()) {
            return;
        }
        if (cm.command_exist(command_str)) {
            try {
                std::function<void(std::vector<std::string>)> &_fn = this->cm.get_val(command_str);
                _fn(parameters);
            } catch (...) {
                
            }
        }
        else {
            std::cout << "Command not found." << std::endl;
        }
    }

public:
    void registerCommand(const std::string& _command_str, const std::function<void(std::vector<std::string>)>_function) {
        this->cm.insert_command(_command_str, _function);
    }

    void run() {
        std::string current_command;
        int command_cursor = 1;
        clear_buffer();
        while (true) {
            char c;
            KEYBOARD key = keyboard_input(c);
            if (key == KEYBOARD::TAB) {
                std::string prefix = cm.longest_prefix(current_command);
                current_command = prefix.empty() ? current_command : prefix;
                flush_print(current_command);
            }
            else if (key == KEYBOARD::ENTER) {
                std::cout << std::endl;
                execute_command(current_command);
                command_history.push_back(current_command);
                command_cursor = command_history.size();
                current_command.clear();
                clear_buffer();
            }
            else if (key == KEYBOARD::BACKSPACE && !current_command.empty()) {
                current_command.pop_back();
                flush_print(current_command);
            }
            else if (key == KEYBOARD::UP) {
                current_command = command_history[command_cursor > 0 ? --command_cursor : 0];
                flush_print(current_command);
            }
            else if (key == KEYBOARD::DOWN) {
                int n = command_history.size();
                current_command = command_cursor < n - 1 ? command_history[command_cursor + 1] : "";
                command_cursor = command_cursor < n ? command_cursor + 1 : command_cursor;
                flush_print(current_command);
            }
            else if (key == KEYBOARD::ALPHA){
                current_command.push_back(c);
                flush_print(current_command);
            }
            else if (key == KEYBOARD::EXIT) {
                break;
            }
        }
    }
};

}
