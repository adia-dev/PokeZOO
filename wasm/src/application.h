#pragma once

#include "sprite.h"

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

	static std::vector<std::unique_ptr<Sprite>> &get_entities() { return instance()->_sprites; }

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
	 * Game state
	 */
	bool                                 _running         = false;
	int                                  _width           = 1920;
	int                                  _height          = 1080;
	int                                  _last_frame_time = 0;
	int                                  _delta_time      = 0;
	std::vector<std::unique_ptr<Sprite>> _sprites;
	std::unique_ptr<Sprite>              _player = nullptr;
};
