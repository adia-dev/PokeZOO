#include "application.hpp"

void Application::run()
{
    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_CreateWindowAndRenderer(600, 400, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    if (init() == 0)
    {
        printf("Failed to initialize the context\n");
        return;
    }
    key_active_state = NOTHING_PRESSED;
    dest.x = 200;
    dest.y = 100;
    pos_x = 0;
    pos_y = 0;

    /**
     * Schedule the main loop handler to get
     * called on each animation frame
     */
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg([](void *arg)
                                 {
    auto* ctx = static_cast<Application*>(arg);
    ctx->loop_handler(); },
                                 this, -1, 1);
#else
    while (is_running)
    {
        loop_handler();
    }
#endif
}

int Application::init()
{
    SDL_Surface *image = IMG_Load("../src/assets/images/characters_no_bg.png");
    if (!image)
    {
        printf("IMG_Load: %s\n", IMG_GetError());
        return 0;
    }
    spritsheet_tex = SDL_CreateTextureFromSurface(renderer, image);
    dest.w = image->w;
    dest.h = image->h;

    frame_rect.w = 32;
    frame_rect.h = 32;
    frame_rect.x = 0;
    frame_rect.y = 0;

    frame = 0;
    frame_count = 3;
    frame_delay = 100;
    frame_timer = 0;

    last_tick = SDL_GetTicks();

    font = TTF_OpenFont("../src/assets/fonts/Roboto/Roboto-Regular.ttf", 24);
    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface *text_surface = TTF_RenderText_Solid(font, "Choso Goat", color);
    if (!text_surface)
    {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        return 0;
    }

    text_tex = SDL_CreateTextureFromSurface(renderer, text_surface);

    SDL_FreeSurface(image);
    SDL_FreeSurface(text_surface);

    return 1;
}

void Application::handle_events()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {

        if (event.type == SDL_QUIT)
        {
            is_running = false;
            break;
        }

        switch (event.key.keysym.sym)
        {
        case SDLK_UP:
            if (event.key.type == SDL_KEYDOWN)
                key_active_state |= static_cast<input_state>(UP_PRESSED);
            else if (event.key.type == SDL_KEYUP)
                key_active_state ^= static_cast<input_state>(UP_PRESSED);
            break;
        case SDLK_DOWN:
            if (event.key.type == SDL_KEYDOWN)
                key_active_state |= static_cast<input_state>(DOWN_PRESSED);
            else if (event.key.type == SDL_KEYUP)
                key_active_state ^= static_cast<input_state>(DOWN_PRESSED);
            break;
        case SDLK_LEFT:
            if (event.key.type == SDL_KEYDOWN)
                key_active_state |= static_cast<input_state>(LEFT_PRESSED);
            else if (event.key.type == SDL_KEYUP)
                key_active_state ^= static_cast<input_state>(LEFT_PRESSED);
            break;
        case SDLK_RIGHT:
            if (event.key.type == SDL_KEYDOWN)
                key_active_state |= static_cast<input_state>(RIGHT_PRESSED);
            else if (event.key.type == SDL_KEYUP)
                key_active_state ^= (enum input_state)(RIGHT_PRESSED);
            break;
        case SDLK_ESCAPE:
            is_running = false;
            break;
        default:
            break;
        }
    }
}

void Application::loop_handler()
{
    dt = SDL_GetTicks() - last_tick;

    int vx = 0;
    int vy = 0;

    key_active_state = NOTHING_PRESSED;
    handle_events();

    dest.x += pos_x;
    dest.y += pos_y;

    SDL_Rect text_dest = {dest.x, dest.y - 30, 100, 30};

    frame_timer += dt;
    if (frame_timer > frame_delay)
    {
        frame_timer = 0;
        frame++;
        if (frame >= frame_count)
        {
            frame = 0;
        }
        frame_rect.x = frame_rect.w * frame;
    }

    last_tick = SDL_GetTicks();

    // move the destination rect
    if (key_active_state & UP_PRESSED)
    {
        vy -= 1;
    }
    if (key_active_state & DOWN_PRESSED)
    {
        vy += 1;
    }
    if (key_active_state & LEFT_PRESSED)
    {
        vx -= 1;
    }
    if (key_active_state & RIGHT_PRESSED)
    {
        vx += 1;
    }

    if (vx != 0 || vy != 0)
    {
        pos_x = vx;
        pos_y = vy;
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, spritsheet_tex, &frame_rect, &dest);
    SDL_RenderCopy(renderer, text_tex, NULL, &text_dest);
    SDL_RenderPresent(renderer);
}

Application::~Application()
{
    SDL_DestroyTexture(spritsheet_tex);
    SDL_DestroyTexture(text_tex);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    TTF_Quit();
}
