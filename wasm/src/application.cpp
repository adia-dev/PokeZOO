#include "application.h"

#include "input_handler.h"

Application::Application() {}

Application::~Application() {
	//? NOTE: window and renderer are smart pointers, so they will be destroyed
	// automatically

	SDL_Quit();
	TTF_Quit();
}

int Application::run() {
	std::shared_ptr<Application> app = instance();

	if (app == nullptr) {
		return 1;
	}

	if (!app->init()) {
		return 1;
	}

	if (!app->load_assets()) {
		return 1;
	}

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(
	    [](void *arg) {
		    Application *app = static_cast<Application *>(arg);
		    app->handle_events();
		    app->handle_input();
		    app->update_delta_time();
		    app->update();
		    app->render();
	    },
	    app.get(),
	    -1,
	    1);
#else
	while (app->_running) {
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

bool Application::init() {
	SDL_CreateWindowAndRenderer(_width,
	                            _height,
	                            SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED,
	                            (SDL_Window **)&_window,
	                            (SDL_Renderer **)&_renderer);

	if (_window == nullptr || _renderer == nullptr) {
		printf("Failed to create window and renderer: %s\n", SDL_GetError());
		SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
		return false;
	}

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Failed to initialise SDL: %s\n", SDL_GetError());
		SDL_Log("Failed to initialise SDL: %s", SDL_GetError());
		return false;
	}

	if (TTF_Init() != 0) {
		printf("Failed to initialise SDL_ttf: %s\n", SDL_GetError());
		SDL_Log("Failed to initialise SDL_ttf: %s", SDL_GetError());
		return false;
	}

	_running = true;

	printf("SDL initialised successfully\n");

	return true;
}

bool Application::load_assets() {
	return true;
}

void Application::handle_events() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
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

void Application::on_loop_start() {
	InputHandler::update_key_states();
}

void Application::handle_input() {}

void Application::handle_key_down(SDL_Keycode key) {
	if (key == SDLK_ESCAPE) {
		quit();
	}

	InputHandler::set_key_state(key, InputHandler::is_key_pressed(key) ? KeyState::DOWN : KeyState::PRESSED);
}

void Application::handle_key_up(SDL_Keycode key) {
	InputHandler::set_key_state(key, KeyState::RELEASED);
}

void Application::update_delta_time() {
	int current_time = SDL_GetTicks();
	_delta_time      = (current_time - _last_frame_time) / 1000.0f;
	_last_frame_time = current_time;
}

void Application::update() {
	for (auto [code, state] : InputHandler::get_key_states()) {
		if (state == KeyState::PRESSED) {
			printf("%s: %s\n",
			       InputHandler::key_code_to_string(code).c_str(),
			       InputHandler::key_state_to_string(state).c_str());
		}
	}
}

void Application::render() {
	if (_renderer == nullptr || _window == nullptr) {
		return;
	}

	SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
	SDL_RenderClear(_renderer.get());

	SDL_Rect frame = {0, 0, 32, 32};
	SDL_RenderCopy(_renderer.get(),
	               &AssetManager::get_texture("../src/assets/images/characters_no_bg.png"),
	               &frame,
	               &frame);

	SDL_RenderPresent(_renderer.get());
}

void Application::render_text(const char *text, int x, int y, int size) {}

void Application::quit() {
	std::shared_ptr<Application> app = instance();
	app->_running                    = false;

#ifdef __EMSCRIPTEN__
	emscripten_cancel_main_loop();
#endif
}
