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

    while (app->_running)
    {
        app->handle_events();
        app->handle_input();
        app->update_delta_time();
        app->update();
        app->render();
    }

    return 0;
}

bool Application::init()
{

    _window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(
        SDL_CreateWindow("SDL2 Boilerplate", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 640, 480, 0),
        SDL_DestroyWindow);

    if (_window == nullptr)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    _renderer = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(
        SDL_CreateRenderer(_window.get(), -1,
                           SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
        SDL_DestroyRenderer);

    if (_renderer == nullptr)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Failed to initialise SDL: %s", SDL_GetError());
        return false;
    }

    if (TTF_Init() != 0)
    {
        SDL_Log("Failed to initialise SDL_ttf: %s", SDL_GetError());
        return false;
    }

    _running = true;

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
