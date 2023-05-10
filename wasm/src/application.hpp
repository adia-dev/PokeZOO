#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <emscripten.h>
#include <stdio.h>

/**
 * Inverse square root of two, for normalising velocity
 */
#define REC_SQRT2 0.7071067811865475

/**
 * Set of input states
 */
enum input_state
{
    NOTHING_PRESSED = 0,
    UP_PRESSED = 1,
    DOWN_PRESSED = 1 << 1,
    LEFT_PRESSED = 1 << 2,
    RIGHT_PRESSED = 1 << 3
};

class Application
{
public:
    Application() {}
    ~Application();

    void run();

private:
    SDL_Renderer *renderer_;

    /**
     * Rectangle that the choso texture will be rendered into
     */
    SDL_Rect dest_;
    SDL_Texture *spritsheet_tex_;

    /**
     * Font used to render the text
     * Textures are used to render text in SDL
     */
    TTF_Font *font_;
    SDL_Texture *text_tex_;

    // enum input_state key_active_state_;
    // use a uint8_t instead of enum to avoid
    // compiler warnings
    uint8_t key_active_state_;

    /**
     * Position
     */
    int pos_x_;
    int pos_y_;

    /**
     * Current frame
     */
    int frame_;
    int frame_count_;
    int frame_delay_;
    int frame_timer_;
    SDL_Rect frame_rect_;

    /**
     * Delta time
     */
    int dt_;
    int last_tick_;

    /**
     * Loads the choso texture into the context
     */
    int init();

    /**
     * Processes the input events and sets the velocity
     * of the choso accordingly
     */
    void handle_events();

    /**
     * Loop handler that gets called each animation frame,
     * process the input, update the position of the choso and
     * then render the texture
     */
    void loop_handler();
};
