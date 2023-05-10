#include "application.hpp"

extern "C" int mainf()
{
    Application app;
    app.run();
    return 0;
}

int main()
{
    return mainf();
}