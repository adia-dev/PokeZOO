#pragma once

#include "animation_controller.h"

#include <functional>

class Sprite {
  public:
	Sprite(SDL_Texture& texture, const SDL_Rect& frame_rect, const SDL_Rect& world_rect);
	Sprite(SDL_Texture& texture, const SDL_Rect& frame_rect, int x, int y, int w, int h);
	Sprite(const Sprite& other);
	virtual ~Sprite() = default;

	virtual void render(SDL_Renderer* renderer);
	virtual void update(float delta_time);

	virtual void move(int x, int y);

	void set_position(int x, int y);
	void set_size(int w, int h);

	int get_x() const { return _bounding_rect.x; }
	int get_y() const { return _bounding_rect.y; }
	int get_w() const { return _bounding_rect.w; }
	int get_h() const { return _bounding_rect.h; }

	void set_x(int x) { _bounding_rect.x = x; }
	void set_y(int y) { _bounding_rect.y = y; }
	void set_w(int w) { _bounding_rect.w = w; }
	void set_h(int h) { _bounding_rect.h = h; }

	void set_speed(float speed) { _speed = speed; }

	void set_is_moving(bool is_moving) { _is_moving = is_moving; }
	bool get_is_moving() const { return _is_moving; }

	const SDL_Rect& get_frame_rect() const { return _frame_rect; }
	const SDL_Rect& get_bounding_rect() const { return _bounding_rect; }
	// TODO: use a variable for the tile size
	Vector2i get_coords() const { return Vector2i(_bounding_rect.x / TILE_SIZE, _bounding_rect.y / TILE_SIZE); }

	bool is_colliding(const Sprite& other) const;
	bool is_colliding(const SDL_Rect& rect) const;

	void handle_collision(const Sprite& other);
	void handle_collision(const SDL_Rect& rect);

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
		os << "  \"bounding_rect\": {\n";
		os << "    \"x\": " << sprite._bounding_rect.x << ",\n";
		os << "    \"y\": " << sprite._bounding_rect.y << ",\n";
		os << "    \"w\": " << sprite._bounding_rect.w << ",\n";
		os << "    \"h\": " << sprite._bounding_rect.h << "\n";
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
			case Direction::NONE:
			default:
				os << "\"NONE\"";
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
	SDL_Rect     _bounding_rect;

	Direction _direction = Direction::DOWN;

	bool  _is_moving = false;
	float _speed     = 0.0f;

	AnimationController _animation_controller;
};
