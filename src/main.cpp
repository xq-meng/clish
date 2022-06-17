#include <iostream>
#include <string>
#include <stack>

#include "command_list.h"
#include "command_shell.h"

using namespace std;

command_list command_list_initialize() {
    command_list cst;
    cst.insert_command("Create::Connector");
    cst.insert_command("Create::Domain");
    return cst;
}


int main(int argc, char** argv)
{
    command_list cst = command_list_initialize();

    shell::run(cst);

    return 0;
}
