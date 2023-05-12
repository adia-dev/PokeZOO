#include "sprite.h"

#include <SDL_render.h>

Sprite::Sprite(SDL_Texture& texture, const SDL_Rect& frame_rect, const SDL_Rect& world_rect)
    : _texture(texture), _frame_rect(frame_rect), _world_rect(world_rect) {}

Sprite::Sprite(SDL_Texture& texture, const SDL_Rect& frame_rect, int x, int y, int w, int h)
    : _texture(texture), _frame_rect(frame_rect) {
	_world_rect.x = x;
	_world_rect.y = y;
	_world_rect.w = w;
	_world_rect.h = h;
}

Sprite::Sprite(const Sprite& other)
    : _texture(other._texture), _frame_rect(other._frame_rect), _world_rect(other._world_rect) {}

void Sprite::render(SDL_Renderer* renderer) {
	if (renderer == NULL) return;

	SDL_RenderCopy(renderer, &_texture, &_frame_rect, &_world_rect);
}

void Sprite::update(float delta_time) {
	if (_animation_controller.has_animations()) {
		_animation_controller.update(delta_time);
		_frame_rect = _animation_controller.get_current_frame().rect;
	}
}

void Sprite::move(int x, int y) {
	_world_rect.x += x;
	_world_rect.y += y;
}

void Sprite::set_position(int x, int y) {
	_world_rect.x = x;
	_world_rect.y = y;
}

void Sprite::set_size(int w, int h) {
	_world_rect.w = w;
	_world_rect.h = h;
}
