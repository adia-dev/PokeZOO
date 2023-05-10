#include "application.hpp"

extern "C" EMSCRIPTEN_KEEPALIVE int mainf()
{
    Application app;
    app.run();
    return 0;
}