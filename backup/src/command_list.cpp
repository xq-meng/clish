#include "command_list.h"

command_list::command_list() {
    this->command_id = 0;
}

bool command_list::command_exist(std::string&& _command_str) const {
    return this->command_st.count(_command_str);
}

bool command_list::command_exist(const std::string& _command_str) const {
    return this->command_st.count(_command_str);
}

void command_list::insert_command(std::string&& _command_str) {
    this->command_st.insert(_command_str, this->command_id++);
}

std::vector<std::string> command_list::commands_with_prefix(const std::string& _prefix) const {
    auto prefix_range = this->command_st.equal_prefix_range(_prefix);
    std::vector<std::string> ret;
    for (auto it = prefix_range.first; it != prefix_range.second; ++it) {
        ret.push_back(std::string(it.key()));
    }
    return ret;
}

std::vector<std::string> command_list::commands_with_prefix(std::string&& _prefix) const {
    auto prefix_range = this->command_st.equal_prefix_range(_prefix);
    std::vector<std::string> ret;
    for (auto it = prefix_range.first; it != prefix_range.second; ++it) {
        ret.push_back(std::string(it.key()));
    }
    return ret;
}

std::string command_list::longest_prefix(const std::string& _prefix) const {
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

std::string command_list::longest_prefix(std::string&& _prefix) const {
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
