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

    int pos = 0;

    inline void flush_print(const std::string& to_print) {
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

    inline void clear_buffer() {
        std::cout << ">>> " << std::flush;
        pos = 0;
    }

    void execute_command(const std::string& command) {
        std::stringstream ss(command);
        std::string word;
        std::vector<std::string> command_para_list;
        while (ss >> word) {
            command_para_list.push_back(word);
        }
        if (command_para_list.empty()) {
            return;
        }
        if (cm.command_exist(command_para_list[0])) {
            try {
                std::vector<std::string> parameters;
                for (int i = 1; i < command_para_list.size(); i++)
                    parameters.emplace_back(command_para_list[i]);
                std::function<void(std::vector<std::string>)> &_fn = this->cm.get_val(command);
                _fn(parameters);
            } catch (std::exception &e) {
                
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
        std::vector<std::string> command_history = { "" }; 
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
