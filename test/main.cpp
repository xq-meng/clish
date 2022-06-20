#include "clish/clish.h"

using namespace std;

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
    std::function<void(std::vector<std::string>)> parameter_print = fb;

    cl.registerCommand("Lambda::Foo", standard_lambda_foo);
    cl.registerCommand("Lambda::Bar", standard_lambda_bar);
    cl.registerCommand("Print::Parameter", fb);

    cl.run();

    return 0;
}
