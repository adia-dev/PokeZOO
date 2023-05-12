#include "application.h"

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

	if (!app->init_entities()) {
		return 1;
	}

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(
	    [](void *arg) {
		    Application *app = static_cast<Application *>(arg);
		    app->on_loop_start();
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
		app->on_loop_start();
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
	if (!AssetManager::load_texture("../src/assets/images/characters_no_bg.png")) return false;

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
			// mouse events
			case SDL_MOUSEMOTION:
				handle_mouse_motion(event.motion.x, event.motion.y);
				break;
			case SDL_MOUSEBUTTONDOWN:
				handle_mouse_button_down(event.button.button, event.button.x, event.button.y);
				break;
			case SDL_MOUSEBUTTONUP:
				handle_mouse_button_up(event.button.button, event.button.x, event.button.y);
				break;
			case SDL_MOUSEWHEEL:
				handle_mouse_wheel(event.wheel.x, event.wheel.y);
				break;
		}
	}
}

bool Application::init_entities() {
	//? NOTE: this is where you would initialise your entities
	//? e.g.:
	for (int i = 0; i < 100; ++i) {
		Application::add_entity(Sprite(AssetManager::get_texture("../src/assets/images/characters_no_bg.png"),
		                               {0, 141, 32, 32},
		                               {rand() % 1920, rand() % 1080, 32, 32}));
	}
	printf("%zu Entities created !\n", Application::get_entities().size());

	_player = std::make_unique<Sprite>(
	    Sprite(AssetManager::get_texture("../src/assets/images/characters_no_bg.png"), {0, 0, 32, 32}, {0, 0, 32, 32}));

	Animation idle_up_animation    = Animation("idle_up", {0, 0, 32, 32}, 1, 1);
	Animation idle_down_animation  = Animation("idle_down", {0, 32, 32, 32}, 1, 1);
	Animation idle_left_animation  = Animation("idle_left", {0, 64, 32, 32}, 1, 1);
	Animation idle_right_animation = Animation("idle_right", {0, 96, 32, 32}, 1, 1);

	std::vector<AnimationFrame> walk_up_frames;
	walk_up_frames.push_back(AnimationFrame({32, 0, 32, 32}, 100));
	walk_up_frames.push_back(AnimationFrame({0, 0, 32, 32}, 100));
	walk_up_frames.push_back(AnimationFrame({64, 0, 32, 32}, 100));
	walk_up_frames.push_back(AnimationFrame({0, 0, 32, 32}, 100));
	Animation walk_up_animation = Animation("walk_up", walk_up_frames, AnimationDirection::LOOP);

	std::vector<AnimationFrame> walk_down_frames;
	walk_down_frames.push_back(AnimationFrame({32, 32, 32, 32}, 100));
	walk_down_frames.push_back(AnimationFrame({0, 32, 32, 32}, 100));
	walk_down_frames.push_back(AnimationFrame({64, 32, 32, 32}, 100));
	walk_down_frames.push_back(AnimationFrame({0, 32, 32, 32}, 100));
	Animation walk_down_animation = Animation("walk_down", walk_down_frames, AnimationDirection::LOOP);

	std::vector<AnimationFrame> walk_left_frames;
	walk_left_frames.push_back(AnimationFrame({32, 64, 32, 32}, 100));
	walk_left_frames.push_back(AnimationFrame({0, 64, 32, 32}, 100));
	walk_left_frames.push_back(AnimationFrame({64, 64, 32, 32}, 100));
	walk_left_frames.push_back(AnimationFrame({0, 64, 32, 32}, 100));
	Animation walk_left_animation = Animation("walk_left", walk_left_frames, AnimationDirection::LOOP);

	std::vector<AnimationFrame> walk_right_frames;
	walk_right_frames.push_back(AnimationFrame({32, 96, 32, 32}, 100));
	walk_right_frames.push_back(AnimationFrame({0, 96, 32, 32}, 100));
	walk_right_frames.push_back(AnimationFrame({64, 96, 32, 32}, 100));
	walk_right_frames.push_back(AnimationFrame({0, 96, 32, 32}, 100));
	Animation walk_right_animation = Animation("walk_right", walk_right_frames, AnimationDirection::LOOP);

	_player->get_animation_controller().add_animation("idle_up", idle_up_animation);
	_player->get_animation_controller().add_animation("idle_down", idle_down_animation);
	_player->get_animation_controller().add_animation("idle_left", idle_left_animation);
	_player->get_animation_controller().add_animation("idle_right", idle_right_animation);

	_player->get_animation_controller().add_animation("walk_up", walk_up_animation);
	_player->get_animation_controller().add_animation("walk_down", walk_down_animation);
	_player->get_animation_controller().add_animation("walk_left", walk_left_animation);
	_player->get_animation_controller().add_animation("walk_right", walk_right_animation);

	_player->get_animation_controller().play("walk_right");

	_player->set_position(1920 / 2 - 16, 1080 / 2 - 16);

	return true;
}

void Application::add_entity(const Sprite &sprite) {
	auto app = Application::instance();

	app->get_entities().push_back(std::make_unique<Sprite>(sprite));
}

void Application::on_loop_start() {
	InputHandler::update_key_states();
	InputHandler::update_mouse_states();
}

void Application::handle_input() {
	auto input_direction = InputHandler::get_key_direction();

	// set the player's direction
	if (input_direction.x > 0) {
		_player->set_direction(Direction::RIGHT);
	} else if (input_direction.x < 0) {
		_player->set_direction(Direction::LEFT);
	} else if (input_direction.y > 0) {
		_player->set_direction(Direction::DOWN);
	} else if (input_direction.y < 0) {
		_player->set_direction(Direction::UP);
	}

	if (input_direction.magnitude() > 0.1f) {
		_player->get_animation_controller().play(
		    "walk_" + StringUtils::to_lower(InputHandler::direction_to_string(_player->get_direction())));
		_player->move(input_direction.x * _delta_time * 300.f, input_direction.y * _delta_time * 300.f);
	} else {
		_player->get_animation_controller().play(
		    "idle_" + StringUtils::to_lower(InputHandler::direction_to_string(_player->get_direction())));
	}
}

void Application::handle_key_down(SDL_Keycode key) {
	if (key == SDLK_ESCAPE) {
		quit();
	}

	InputHandler::set_key_state(key, InputState::PRESSED);
}

void Application::handle_key_up(SDL_Keycode key) {
	InputHandler::set_key_state(key, InputState::RELEASED);
}

void Application::handle_mouse_motion(int x, int y) {
	InputHandler::set_mouse_position(x, y);
}

void Application::handle_mouse_button_down(Uint8 button, int x, int y) {
	InputHandler::set_mouse_button_state(InputHandler::uint8_to_mouse_button(button), InputState::PRESSED);
}

void Application::handle_mouse_button_up(Uint8 button, int x, int y) {
	InputHandler::set_mouse_button_state(InputHandler::uint8_to_mouse_button(button), InputState::RELEASED);
}

void Application::handle_mouse_wheel(int x, int y) {
	InputHandler::set_mouse_wheel(x, y);
}

void Application::update_delta_time() {
	double current_time = SDL_GetTicks();
	_delta_time         = (current_time - _last_frame_time) / 1000.0f;
	_last_frame_time    = current_time;
}

void Application::update() {
	for (auto &sprite : _sprites) {
		sprite->update(_delta_time);
	}

	_player->update(_delta_time);
}

void Application::render() {
	if (_renderer == nullptr || _window == nullptr) {
		return;
	}

	SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
	SDL_RenderClear(_renderer.get());

	render_background();

	for (auto &sprite : _sprites) {
		sprite->render(_renderer.get());
	}

	_player->render(_renderer.get());

	// write the delta time to the screen
	std::stringstream ss;
	ss << "Delta Time: " << _delta_time << " FPS: " << 1.0f / _delta_time << std::endl
	   << "Inputs: " << InputHandler::get() << std::endl
	   << "Player Animation Controller: " << _player->get_animation_controller();

	SDL_Color    color = {255, 255, 255, 255};
	SDL_Surface *surface =
	    TTF_RenderText_Blended_Wrapped(&AssetManager::get_font("../src/assets/fonts/Roboto/Roboto-Regular.ttf", 16),
	                                   ss.str().c_str(),
	                                   color,
	                                   1920);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer.get(), surface);
	SDL_Rect     rect    = {0, 0, surface->w, surface->h};
	SDL_RenderCopy(_renderer.get(), texture, NULL, &rect);
	SDL_FreeSurface(surface);

	// render a red rectangle at the mouse position, 32x32 closest grid square
	SDL_SetRenderDrawColor(_renderer.get(), 255, 0, 0, 255);
	SDL_Rect rect2 = {(int)InputHandler::get_mouse_position().x / 32 * 32,
	                  (int)InputHandler::get_mouse_position().y / 32 * 32,
	                  32,
	                  32};
	SDL_RenderDrawRect(_renderer.get(), &rect2);

	SDL_RenderPresent(_renderer.get());
}

void Application::render_background() {
	// Draw a 32x32 grid, only outlines and low alpha
	for (int x = 0; x < 1920; x += 32) {
		for (int y = 0; y < 1080; y += 32) {
			SDL_SetRenderDrawColor(_renderer.get(), 88, 88, 88, 32);
			SDL_RenderDrawLine(_renderer.get(), x, 0, x, 1080);
			SDL_RenderDrawLine(_renderer.get(), 0, y, 1920, y);
		}
	}
}

void Application::render_text(const char *text, int x, int y, int size) {}

void Application::quit() {
	std::shared_ptr<Application> app = instance();
	app->_running                    = false;

#ifdef __EMSCRIPTEN__
	emscripten_cancel_main_loop();
#endif
}
