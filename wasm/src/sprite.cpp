#include "sprite.h"

#include <SDL_render.h>

Sprite::Sprite(SDL_Texture& texture, const SDL_Rect& frame_rect, const SDL_Rect& world_rect)
    : _texture(texture), _frame_rect(frame_rect), _bounding_rect(world_rect) {}

Sprite::Sprite(SDL_Texture& texture, const SDL_Rect& frame_rect, int x, int y, int w, int h)
    : _texture(texture), _frame_rect(frame_rect) {
	_bounding_rect.x = x;
	_bounding_rect.y = y;
	_bounding_rect.w = w;
	_bounding_rect.h = h;
}

Sprite::Sprite(const Sprite& other)
    : _texture(other._texture), _frame_rect(other._frame_rect), _bounding_rect(other._bounding_rect),
      _animation_controller(other._animation_controller), _direction(other._direction) {}

void Sprite::render(SDL_Renderer* renderer) {
	if (renderer == NULL) return;

	SDL_RenderCopy(renderer, &_texture, &_frame_rect, &_bounding_rect);
}

void Sprite::update(float delta_time) {
	if (_animation_controller.has_animations()) {
		_animation_controller.update(delta_time);
		_frame_rect = _animation_controller.get_current_frame().rect;
	}
}

void Sprite::move(int x, int y) {
	_bounding_rect.x += x;
	_bounding_rect.y += y;
}

void Sprite::set_position(int x, int y) {
	_bounding_rect.x = x;
	_bounding_rect.y = y;
}

void Sprite::set_size(int w, int h) {
	_bounding_rect.w = w;
	_bounding_rect.h = h;
}

bool Sprite::is_colliding(const Sprite& other) const {
	return is_colliding(other._bounding_rect);
}

bool Sprite::is_colliding(const SDL_Rect& rect) const {
	// AABB collision detection
	return _bounding_rect.x < rect.x + rect.w && _bounding_rect.x + _bounding_rect.w > rect.x &&
	       _bounding_rect.y < rect.y + rect.h && _bounding_rect.y + _bounding_rect.h > rect.y;
}

void Sprite::handle_collision(const Sprite& other) {
	handle_collision(other.get_bounding_rect());
}

void Sprite::handle_collision(const SDL_Rect& rect) {
	// Calculate the minimum translation vector (MTV)
	int dx = 0, dy = 0;

	if (_bounding_rect.x < rect.x) {
		dx = rect.x - (_bounding_rect.x + _bounding_rect.w);
	} else {
		dx = rect.x + rect.w - _bounding_rect.x;
	}

	if (_bounding_rect.y < rect.y) {
		dy = rect.y - (_bounding_rect.y + _bounding_rect.h);
	} else {
		dy = rect.y + rect.h - _bounding_rect.y;
	}

	// Move the sprite out of the collision by adjusting its position
	if (abs(dx) < abs(dy)) {
		// Adjust horizontally
		_bounding_rect.x += dx;
	} else {
		// Adjust vertically
		_bounding_rect.y += dy;
	}
}