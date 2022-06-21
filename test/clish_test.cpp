#include "clish/clish.h"

#include <ctime>

using namespace std;

void prt(std::vector<std::string> args) {
    time_t now = time(0);

    char* time_str = ctime(&now);
    
    if (!args.empty() && args[0] == "UTC") {
        tm* gmt = gmtime(&now);
        time_str = asctime(gmt);
    }

    std::cout << time_str << std::endl;
}


int main(int argc, char** argv)
{
    clish::clish cl;

    auto foo = [](std::vector<std::string>){ std::cout << "Lambda Foo." << std::endl; };
    auto bar = [](std::vector<std::string>){ std::cout << "Lambda Bar." << std::endl; };
    auto fb = [](std::vector<std::string> strs) {
        for (auto& _str : strs)
            std::cout << _str << std::endl;
    };

    std::function<void(std::vector<std::string>)> standard_lambda_foo = foo;
    std::function<void(std::vector<std::string>)> standard_lambda_bar = bar;

    cl.registerCommand("Lambda::Foo", standard_lambda_foo);
    cl.registerCommand("Lambda::Bar", standard_lambda_bar);
    cl.registerCommand("Print::Parameter", fb);
    cl.registerCommand("Print::Time", prt);

    cl.run();

    return 0;
}
