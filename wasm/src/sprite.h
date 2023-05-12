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

	Direction get_direction() const { return _direction; }
	void      set_direction(Direction direction) { _direction = direction; }

	friend std::ostream& operator<<(std::ostream& os, const Sprite& sprite) {
		os << "{\n";
		os << "  \"frame_rect\": {\n";
		os << "    \"x\": " << sprite._frame_rect.x << ",\n";
		os << "    \"y\": " << sprite._frame_rect.y << ",\n";
		os << "    \"w\": " << sprite._frame_rect.w << ",\n";
		os << "    \"h\": " << sprite._frame_rect.h << "\n";
		os << "  },\n";
		os << "  \"world_rect\": {\n";
		os << "    \"x\": " << sprite._world_rect.x << ",\n";
		os << "    \"y\": " << sprite._world_rect.y << ",\n";
		os << "    \"w\": " << sprite._world_rect.w << ",\n";
		os << "    \"h\": " << sprite._world_rect.h << "\n";
		os << "  },\n";
		os << "  \"direction\": ";
		switch (sprite._direction) {
			case Direction::UP:
				os << "\"UP\"";
				break;
			case Direction::DOWN:
				os << "\"DOWN\"";
				break;
			case Direction::LEFT:
				os << "\"LEFT\"";
				break;
			case Direction::RIGHT:
				os << "\"RIGHT\"";
				break;
		}
		os << ",\n";
		os << "  \"is_moving\": " << std::boolalpha << sprite._is_moving << ",\n";
		os << "  \"speed\": " << std::setprecision(2) << std::fixed << sprite._speed << ",\n";
		os << "  \"animation_controller\": " << sprite._animation_controller << "\n";
		os << "}";
		return os;
	}

  protected:
	SDL_Texture& _texture;
	SDL_Rect     _frame_rect;
	SDL_Rect     _world_rect;

	Direction _direction = Direction::DOWN;

	bool  _is_moving = false;
	float _speed     = 0.0f;

	AnimationController _animation_controller;
};
