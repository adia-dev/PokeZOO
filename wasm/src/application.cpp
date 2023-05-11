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
		// app->on_loop_start();
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
	walk_up_frames.push_back(AnimationFrame({96, 0, 32, 32}, 100));
	walk_up_frames.push_back(AnimationFrame({0, 0, 32, 32}, 100));
	Animation walk_up_animation = Animation("walk_up", walk_up_frames, AnimationDirection::LOOP);

	std::vector<AnimationFrame> walk_down_frames;
	walk_down_frames.push_back(AnimationFrame({32, 32, 32, 32}, 100));
	walk_down_frames.push_back(AnimationFrame({0, 32, 32, 32}, 100));
	walk_down_frames.push_back(AnimationFrame({96, 32, 32, 32}, 100));
	walk_down_frames.push_back(AnimationFrame({0, 32, 32, 32}, 100));
	Animation walk_down_animation = Animation("walk_down", walk_down_frames, AnimationDirection::LOOP);

	std::vector<AnimationFrame> walk_left_frames;
	walk_left_frames.push_back(AnimationFrame({32, 64, 32, 32}, 100));
	walk_left_frames.push_back(AnimationFrame({0, 64, 32, 32}, 100));
	walk_left_frames.push_back(AnimationFrame({96, 64, 32, 32}, 100));
	walk_left_frames.push_back(AnimationFrame({0, 64, 32, 32}, 100));
	Animation walk_left_animation = Animation("walk_left", walk_left_frames, AnimationDirection::LOOP);

	std::vector<AnimationFrame> walk_right_frames;
	walk_right_frames.push_back(AnimationFrame({32, 96, 32, 32}, 100));
	walk_right_frames.push_back(AnimationFrame({0, 96, 32, 32}, 100));
	walk_right_frames.push_back(AnimationFrame({96, 96, 32, 32}, 100));
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

	_player->get_animation_controller().play("idle_right");

	_player->set_position(1920 / 2 - 16, 1080 / 2 - 16);

	return true;
}

void Application::add_entity(const Sprite &sprite) {
	auto app = Application::instance();

	app->get_entities().push_back(std::make_unique<Sprite>(sprite));
}

void Application::on_loop_start() {
	InputHandler::update_key_states();
}

void Application::handle_input() {
	if (InputHandler::is_key_pressed(SDL_KeyCode::SDLK_LEFT)) {
		_player->move(-1, 0);
		_player->get_animation_controller().play("walk_left");
	} else if (InputHandler::is_key_pressed(SDL_KeyCode::SDLK_RIGHT)) {
		_player->move(1, 0);
		_player->get_animation_controller().play("walk_right");
	} else if (InputHandler::is_key_pressed(SDL_KeyCode::SDLK_UP)) {
		_player->move(0, -1);
		_player->get_animation_controller().play("walk_up");
	} else if (InputHandler::is_key_pressed(SDL_KeyCode::SDLK_DOWN)) {
		_player->move(0, 1);
		_player->get_animation_controller().play("walk_down");
	} else {
		_player->get_animation_controller().play("idle_right");
	}
}

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

	// for (auto &sprite : _sprites) {
	// 	sprite->update(_delta_time);
	// }

	_player->update(_delta_time);
}

void Application::render() {
	if (_renderer == nullptr || _window == nullptr) {
		return;
	}

	SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
	SDL_RenderClear(_renderer.get());

	for (auto &sprite : _sprites) {
		sprite->render(_renderer.get());
	}

	_player->render(_renderer.get());

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
