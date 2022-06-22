# clish - A shell for command line interface.

## Usage 

### Via include

Clish is a **header-only** library. You do **not** need to build anything to install. Simply add ``clish/include`` to your include path and include ``clish/clish.h``.

Here is a small demo.

```cpp
#include "clish/clish.h"

using namespace std;

int main(int argc, char** argv)
{
    clish::clish cl;

    auto foo = [](std::vector<std::string>){ std::cout << "Hello." << std::endl; };

    std::function<void(std::vector<std::string>)> standard_lambda_foo = foo;

    cl.registerCommand("::Foo", standard_lambda_foo);

    cl.run();

    return 0;
}
```

If you save this in ``hello.cpp``, then you could compile this with :

```bash
g++ -std=c++11 -I[YOUR_PATH_TO_CLISH] hello.cpp -o hello
```

Running ``./hello`` would then produce a shell with internal command ``::Foo``

```
>>> ::Foo
Hello.
```

If you are running on Windows platform, add macro definition ``WINDOWS_PLATFORM`` before include ``clish/clish.h``.

```cpp
#define WINDOWS_PLATFORM
#include "clish/clish.h"
```

### Via CMake

You cam use ``clish`` in your CMake project by ``add_subdirectory`` command.

```
add_subdirectory([YOUR_PATH_TO_CLISH])

target_link_libraries([YOUR_PROJECT] PRIVATE clish::clish)
```

### demo

![demo](https://blog-static.cnblogs.com/files/xqmeng/clish_demo.gif)

## Dependencies

Thanks for Tessil's [hat-trie](https://github.com/Tessil/hat-trie). It provide a trie with high profermance, and I've include ``hat-trie`` in this project. 
