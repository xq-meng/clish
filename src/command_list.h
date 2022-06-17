#pragma once

#include "tsl/htrie_map.h"
#include <string>
#include <vector>

class command_list {
private:
    tsl::htrie_map<char, int> command_st;
    int command_id;
public:
    command_list();
    bool command_exist(const std::string& _command_str) const;
    bool command_exist(std::string&& _command_str) const;
    void insert_command(std::string&& _command_str);
    std::vector<std::string> commands_with_prefix(const std::string& _prefix) const;
    std::vector<std::string> commands_with_prefix(std::string&& _prefix) const;
    std::string longest_prefix(const std::string& _prefix) const;
    std::string longest_prefix(std::string&& _prefix) const;
};
