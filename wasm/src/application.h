#pragma once

#include "character.h"

#ifdef __EMSCRIPTEN__
#	include <emscripten.h>
#endif

class Application {
  public:
	Application();
	~Application();

	/**
	 * Methods for running and quitting the application
	 */
	static int  run();
	static void quit();

	/**
	 * Singleton Instance
	 */
	static std::shared_ptr<Application> instance() {
		if (_instance == nullptr) {
			_instance = std::make_shared<Application>();
		}
		return _instance;
	}

	/**
	 * Getters for the window and renderer
	 */
	static SDL_Window   *get_window() { return instance()->_window.get(); }
	static SDL_Renderer *get_renderer() { return instance()->_renderer.get(); }

	static std::vector<std::unique_ptr<Sprite>> &get_entities() { return instance()->_entities; }

	static void add_entity(const Sprite &sprite);

  private:
	/**
	 *  Singleton Instance
	 */
	inline static std::shared_ptr<Application> _instance = nullptr;

	/**
	 * Window and renderer, smart pointers
	 */
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>     _window   = {nullptr, SDL_DestroyWindow};
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> _renderer = {nullptr, SDL_DestroyRenderer};

	/**
	 * Methods for initialising SDL and loading assets
	 */
	bool init();
	bool load_assets();

	/**
	 * Methods for initialising entities
	 */
	bool init_entities();

	/**
	 * Methods for handling events and input
	 */
	void on_loop_start();
	void handle_events();
	void handle_input();
	void handle_key_down(SDL_Keycode key);
	void handle_key_up(SDL_Keycode key);
	void handle_mouse_motion(int x, int y);
	void handle_mouse_button_down(Uint8 button, int x, int y);
	void handle_mouse_button_up(Uint8 button, int x, int y);
	void handle_mouse_wheel(int x, int y);

	/**
	 * Methods for updating the game state
	 */
	void update_delta_time();
	void update();

	/**
	 * Methods for rendering the game state
	 */
	void render();
	void render_background();
	void render_text(const char *text, int x, int y, int size);

	/**
	 * Getters and setters for game states
	 */
	bool is_running() const { return _running; }
	void set_running(bool running) { _running = running; }

	int  get_window_width() const { return _window_width; }
	void set_window_width(int window_width) { _window_width = window_width; }

	int  get_window_height() const { return _window_height; }
	void set_window_height(int window_height) { _window_height = window_height; }

	int  get_last_frame_time() const { return _last_frame_time; }
	void set_last_frame_time(int last_frame_time) { _last_frame_time = last_frame_time; }

	double get_delta_time() const { return _delta_time; }
	void   set_delta_time(double delta_time) { _delta_time = delta_time; }

	Uint64 get_NOW() const { return NOW; }
	void   set_NOW(Uint64 time) { NOW = time; }

	Uint64 get_LAST() const { return LAST; }
	void   set_LAST(Uint64 time) { LAST = time; }

	/**
	 * Game state
	 */
	bool                                 _running         = false;
	int                                  _window_width    = WINDOW_WIDTH;
	int                                  _window_height   = WINDOW_HEIGHT;
	double                               _last_frame_time = 0;
	double                               _delta_time      = 0;
	Uint64                               NOW              = SDL_GetPerformanceCounter();
	Uint64                               LAST             = 0;
	std::vector<std::unique_ptr<Sprite>> _entities;
	std::unique_ptr<Character>           _player = nullptr;
};
