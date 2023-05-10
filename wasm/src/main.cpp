#include <iostream>
#include <emscripten.h>

extern "C" EMSCRIPTEN_KEEPALIVE void
mainf()
{
    std::cout << "Hello, world!" << std::endl;
}
