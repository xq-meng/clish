#include "clish/clish.h"

using namespace std;

int main(int argc, char** argv)
{
    clish::clish cl;

    auto f = [](std::vector<std::string>){ std::cout << "Lambda function." << std::endl; };

    std::function<void(std::vector<std::string>)> standard_lambda_function = f;

    cl.registerCommand("Lambda::F1", standard_lambda_function);

    cl.run();

    return 0;
}
