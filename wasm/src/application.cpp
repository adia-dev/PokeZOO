#include "application.hpp"

void Application::run()
{
    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_CreateWindowAndRenderer(600, 400, 0, &window, &renderer_);
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);

    if (init() == 0)
    {
        printf("Failed to initialize the context\n");
        return;
    }
    key_active_state_ = NOTHING_PRESSED;
    dest_.x = 200;
    dest_.y = 100;
    pos_x_ = 0;
    pos_y_ = 0;

    /**
     * Schedule the main loop handler to get
     * called on each animation frame
     */
    // emscripten_set_main_loop_arg([](void *arg)
    //                              {
    // auto* ctx = static_cast<Application*>(arg);
    // ctx->loop_handler(); },
    //                              this, -1, 1);
}

int Application::init()
{
    SDL_Surface *image = IMG_Load("../src/assets/images/characters_no_bg.png");
    if (!image)
    {
        printf("IMG_Load: %s\n", IMG_GetError());
        return 0;
    }
    spritsheet_tex_ = SDL_CreateTextureFromSurface(renderer_, image);
    dest_.w = image->w;
    dest_.h = image->h;

    frame_rect_.w = 32;
    frame_rect_.h = 32;
    frame_rect_.x = 0;
    frame_rect_.y = 0;

    frame_ = 0;
    frame_count_ = 3;
    frame_delay_ = 100;
    frame_timer_ = 0;

    last_tick_ = SDL_GetTicks();

    font_ = TTF_OpenFont("../src/assets/fonts/Roboto/Roboto-Regular.ttf", 24);
    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface *text_surface = TTF_RenderText_Solid(font_, "Choso Goat", color);
    if (!text_surface)
    {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        return 0;
    }

    text_tex_ = SDL_CreateTextureFromSurface(renderer_, text_surface);

    SDL_FreeSurface(image);
    SDL_FreeSurface(text_surface);

    return 1;
}

void Application::handle_events()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_UP:
            if (event.key.type == SDL_KEYDOWN)
                key_active_state_ |= static_cast<input_state>(UP_PRESSED);
            else if (event.key.type == SDL_KEYUP)
                key_active_state_ ^= static_cast<input_state>(UP_PRESSED);
            break;
        case SDLK_DOWN:
            if (event.key.type == SDL_KEYDOWN)
                key_active_state_ |= static_cast<input_state>(DOWN_PRESSED);
            else if (event.key.type == SDL_KEYUP)
                key_active_state_ ^= static_cast<input_state>(DOWN_PRESSED);
            break;
        case SDLK_LEFT:
            if (event.key.type == SDL_KEYDOWN)
                key_active_state_ |= static_cast<input_state>(LEFT_PRESSED);
            else if (event.key.type == SDL_KEYUP)
                key_active_state_ ^= static_cast<input_state>(LEFT_PRESSED);
            break;
        case SDLK_RIGHT:
            if (event.key.type == SDL_KEYDOWN)
                key_active_state_ |= static_cast<input_state>(RIGHT_PRESSED);
            else if (event.key.type == SDL_KEYUP)
                key_active_state_ ^= (enum input_state)(RIGHT_PRESSED);
            break;
        default:
            break;
        }
    }
}

void Application::loop_handler()
{
    dt_ = SDL_GetTicks() - last_tick_;

    int vx = 0;
    int vy = 0;
    handle_events();

    dest_.x += pos_x_;
    dest_.y += pos_y_;

    SDL_Rect text_dest = {dest_.x, dest_.y - 30, 100, 30};

    frame_timer_ += dt_;
    if (frame_timer_ > frame_delay_)
    {
        frame_timer_ = 0;
        frame_++;
        if (frame_ >= frame_count_)
        {
            frame_ = 0;
        }
        frame_rect_.x = frame_rect_.w * frame_;
    }

    last_tick_ = SDL_GetTicks();

    // move the destination rect
    if (key_active_state_ & UP_PRESSED)
    {
        vy -= 1;
    }
    if (key_active_state_ & DOWN_PRESSED)
    {
        vy += 1;
    }
    if (key_active_state_ & LEFT_PRESSED)
    {
        vx -= 1;
    }
    if (key_active_state_ & RIGHT_PRESSED)
    {
        vx += 1;
    }

    if (vx != 0 || vy != 0)
    {
        pos_x_ = vx;
        pos_y_ = vy;
    }

    SDL_RenderClear(renderer_);
    SDL_RenderCopy(renderer_, spritsheet_tex_, &frame_rect_, &dest_);
    SDL_RenderCopy(renderer_, text_tex_, NULL, &text_dest);
    SDL_RenderPresent(renderer_);
}

Application::~Application()
{
    SDL_DestroyTexture(spritsheet_tex_);
    SDL_DestroyTexture(text_tex_);
    SDL_DestroyRenderer(renderer_);
    SDL_Quit();
    TTF_Quit();
}
