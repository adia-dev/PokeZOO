#pragma once

#include "animation_controller.h"

#include <functional>

enum class Direction { UP, DOWN, LEFT, RIGHT };

class Sprite {
  public:
	Sprite(SDL_Texture& texture, const SDL_Rect& frame_rect, const SDL_Rect& world_rect);
	Sprite(SDL_Texture& texture, const SDL_Rect& frame_rect, int x, int y, int w, int h);
	Sprite(const Sprite& other);

	virtual void render(SDL_Renderer* renderer);
	virtual void update(float delta_time);

	virtual void move(int x, int y);

	void set_position(int x, int y);
	void set_size(int w, int h);

	int get_x() const { return _world_rect.x; }
	int get_y() const { return _world_rect.y; }
	int get_w() const { return _world_rect.w; }
	int get_h() const { return _world_rect.h; }

	void set_x(int x) { _world_rect.x = x; }
	void set_y(int y) { _world_rect.y = y; }
	void set_w(int w) { _world_rect.w = w; }
	void set_h(int h) { _world_rect.h = h; }

	void set_speed(float speed) { _speed = speed; }

	void set_is_moving(bool is_moving) { _is_moving = is_moving; }
	bool get_is_moving() const { return _is_moving; }

	AnimationController& get_animation_controller() { return _animation_controller; }

  protected:
	SDL_Texture& _texture;
	SDL_Rect     _frame_rect;
	SDL_Rect     _world_rect;

	Direction _direction = Direction::DOWN;

	bool  _is_moving = false;
	float _speed     = 0.0f;

	AnimationController _animation_controller;
};
