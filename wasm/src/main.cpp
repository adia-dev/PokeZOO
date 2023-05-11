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
    if (Application::run() != 0)
    {
        printf("Application failed to run\n");
        return 1;
    }

    return 0;
}
#endif