#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <memory>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

class Application
{
public:
    Application();
    ~Application();

    static int run();

    /**
     * Singleton Instance
     */
    static std::shared_ptr<Application> instance()
    {
        if (_instance == nullptr)
        {
            _instance = std::make_shared<Application>();
        }
        return _instance;
    }

private:
    /**
     *  Singleton Instance
     */
    inline static std::shared_ptr<Application> _instance = nullptr;

    /**
     * Window and renderer, smart pointers
     */
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window = {nullptr, SDL_DestroyWindow};
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> _renderer = {nullptr, SDL_DestroyRenderer};

    /**
     * Methods for initialising SDL and loading assets
     */
    bool init();
    bool load_assets();

    /**
     * Methods for handling events and input
     */
    void handle_events();
    void handle_input();
    void handle_key_down(SDL_Keycode key);
    void handle_key_up(SDL_Keycode key);

    /**
     * Methods for updating the game state
     */
    void update_delta_time();
    void update();

    /**
     * Methods for rendering the game state
     */
    void render();
    void render_text(const char *text, int x, int y, int size);

    /**
     * Methods for cleaning up SDL and assets
     */
    void quit();

    /**
     * Game state
     */
    bool _running = false;
    int _width = 640;
    int _height = 480;
    int _last_frame_time = 0;
    int _delta_time = 0;
};
