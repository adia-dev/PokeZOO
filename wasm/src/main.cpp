#include "application.hpp"

#ifdef __EMSCRIPTEN__
extern "C" EMSCRIPTEN_KEEPALIVE int mainf()
{
    Application app;
    app.run();
    return 0;
}
#else
int main()
{
    Application app;
    app.run();
    return 0;
}
#endif