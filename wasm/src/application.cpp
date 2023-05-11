#include "application.hpp"

Application::Application() {}

Application::~Application()
{
    //? NOTE: window and renderer are smart pointers, so they will be destroyed automatically

    SDL_Quit();
    TTF_Quit();
}

int Application::run()
{
    std::shared_ptr<Application> app = instance();

    if (app == nullptr)
    {
        return 1;
    }

    if (!app->init())
    {
        return 1;
    }

    if (!app->load_assets())
    {
        return 1;
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg([](void *arg)
                                 {
    Application* app = static_cast<Application*>(arg);
        app->handle_events();
        app->handle_input();
        app->update_delta_time();
        app->update();
        app->render(); },
                                 app.get(), -1, 1);
#else
    while (app->_running)
    {
        app->handle_events();
        app->handle_input();
        app->update_delta_time();
        app->update();
        app->render();
    }
#endif

    printf("Application exited successfully\n");

    return 0;
}

bool Application::init()
{

    SDL_CreateWindowAndRenderer(640, 480, 0, (SDL_Window **)&_window, (SDL_Renderer **)&_renderer);

    if (_window == nullptr || _renderer == nullptr)
    {
        printf("Failed to create window and renderer: %s\n", SDL_GetError());
        SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
        return false;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Failed to initialise SDL: %s\n", SDL_GetError());
        SDL_Log("Failed to initialise SDL: %s", SDL_GetError());
        return false;
    }

    if (TTF_Init() != 0)
    {
        printf("Failed to initialise SDL_ttf: %s\n", SDL_GetError());
        SDL_Log("Failed to initialise SDL_ttf: %s", SDL_GetError());
        return false;
    }

    _running = true;

    printf("SDL initialised successfully\n");

    return true;
}

bool Application::load_assets()
{
    return true;
}

void Application::handle_events()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            quit();
            break;
        case SDL_KEYDOWN:
            handle_key_down(event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            handle_key_up(event.key.keysym.sym);
            break;
        }
    }
}

void Application::handle_input()
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    for (int i = 0; i < 512; i++)
    {
        if (state[i])
        {
            printf("%d\n", i);
        }
    }
}

void Application::handle_key_down(SDL_Keycode key)
{
    if (key == SDLK_ESCAPE)
    {
        quit();
    }
}

void Application::handle_key_up(SDL_Keycode key) {}

void Application::update_delta_time()
{
    int current_time = SDL_GetTicks();
    _delta_time = (current_time - _last_frame_time) / 1000.0f;
    _last_frame_time = current_time;
}

void Application::update() {}

void Application::render()
{
    if (_renderer == nullptr || _window == nullptr)
    {
        return;
    }

    SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(_renderer.get());
    SDL_RenderPresent(_renderer.get());
}

void Application::render_text(const char *text, int x, int y, int size)
{
}

void Application::quit()
{
    _running = false;
}