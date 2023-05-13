#include "character.h"

Character::Character(SDL_Texture& texture, const SDL_Rect& frame_rect, const SDL_Rect& world_rect)
    : Sprite(texture, frame_rect, world_rect) {}

Character::Character(SDL_Texture& texture, const SDL_Rect& frame_rect, int x, int y, int w, int h)
    : Sprite(texture, frame_rect, x, y, w, h) {}

Character::Character(const Character& other): Sprite(other) {}

void Character::render(SDL_Renderer* renderer) {
	Sprite::render(renderer);
}

void Character::update(float delta_time) {
	Sprite::update(delta_time);
}

void Character::move(int x, int y) {
	Sprite::move(x, y);
}

void Character::set_on_bike(bool on_bike) {
	_on_bike = on_bike;
}

void Character::toggle_on_bike() {
	_on_bike = !_on_bike;
}

bool Character::get_on_bike() const {
	return _on_bike;
}
